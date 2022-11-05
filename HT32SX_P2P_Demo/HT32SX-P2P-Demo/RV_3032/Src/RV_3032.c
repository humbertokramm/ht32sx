 /******************************************************************************
RV3032.h
Humberto Kramm
Novembro 27, 2022
******************************************************************************/

#include <time.h>

//#include "../Inc/RV3032.h"
#include "RV_3032.h"


uint8_t _time[TIME_ARRAY_LENGTH];
bool _isTwelveHour = true;
I2C_HandleTypeDef  hi2c;

//****************************************************************************//
//
//  Settings and configuration
//
//****************************************************************************//

// Parse the __DATE__ predefined macro to generate date defaults:
// __Date__ Format: MMM DD YYYY (First D may be a space if <10)
// <MONTH>
#define BUILD_MONTH_JAN ((__DATE__[0] == 'J') && (__DATE__[1] == 'a')) ? 1 : 0
#define BUILD_MONTH_FEB (__DATE__[0] == 'F') ? 2 : 0
#define BUILD_MONTH_MAR ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'r')) ? 3 : 0
#define BUILD_MONTH_APR ((__DATE__[0] == 'A') && (__DATE__[1] == 'p')) ? 4 : 0
#define BUILD_MONTH_MAY ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'y')) ? 5 : 0
#define BUILD_MONTH_JUN ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'n')) ? 6 : 0
#define BUILD_MONTH_JUL ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'l')) ? 7 : 0
#define BUILD_MONTH_AUG ((__DATE__[0] == 'A') && (__DATE__[1] == 'u')) ? 8 : 0
#define BUILD_MONTH_SEP (__DATE__[0] == 'S') ? 9 : 0
#define BUILD_MONTH_OCT (__DATE__[0] == 'O') ? 10 : 0
#define BUILD_MONTH_NOV (__DATE__[0] == 'N') ? 11 : 0
#define BUILD_MONTH_DEC (__DATE__[0] == 'D') ? 12 : 0
#define BUILD_MONTH BUILD_MONTH_JAN | BUILD_MONTH_FEB | BUILD_MONTH_MAR | \
BUILD_MONTH_APR | BUILD_MONTH_MAY | BUILD_MONTH_JUN | \
BUILD_MONTH_JUL | BUILD_MONTH_AUG | BUILD_MONTH_SEP | \
BUILD_MONTH_OCT | BUILD_MONTH_NOV | BUILD_MONTH_DEC
// <DATE>
#define BUILD_DATE_0 ((__DATE__[4] == ' ') ? 0 : (__DATE__[4] - 0x30))
#define BUILD_DATE_1 (__DATE__[5] - 0x30)
#define BUILD_DATE ((BUILD_DATE_0 * 10) + BUILD_DATE_1)
// <YEAR>
#define BUILD_YEAR (((__DATE__[7] - 0x30) * 1000) + ((__DATE__[8] - 0x30) * 100) + \
((__DATE__[9] - 0x30) * 10)  + ((__DATE__[10] - 0x30) * 1))

// Parse the __TIME__ predefined macro to generate time defaults:
// __TIME__ Format: HH:MM:SS (First number of each is padded by 0 if <10)
// <HOUR>
#define BUILD_HOUR_0 ((__TIME__[0] == ' ') ? 0 : (__TIME__[0] - 0x30))
#define BUILD_HOUR_1 (__TIME__[1] - 0x30)
#define BUILD_HOUR ((BUILD_HOUR_0 * 10) + BUILD_HOUR_1)
// <MINUTE>
#define BUILD_MINUTE_0 ((__TIME__[3] == ' ') ? 0 : (__TIME__[3] - 0x30))
#define BUILD_MINUTE_1 (__TIME__[4] - 0x30)
#define BUILD_MINUTE ((BUILD_MINUTE_0 * 10) + BUILD_MINUTE_1)
// <SECOND>
#define BUILD_SECOND_0 ((__TIME__[6] == ' ') ? 0 : (__TIME__[6] - 0x30))
#define BUILD_SECOND_1 (__TIME__[7] - 0x30)
#define BUILD_SECOND ((BUILD_SECOND_0 * 10) + BUILD_SECOND_1)


/*bool begin(TwoWire &wirePort)
{
	_i2cPort = &wirePort;
	
	_i2cPort->beginTransmission(RV3032_ADDR);
	
	if (_i2cPort->endTransmission() != 0)
	{
		return (false); //Error: Sensor did not ack
	}
	return(true);
}*/

//Configures the microcontroller to convert to 12 hour mode.
void set12Hour()
{
	_isTwelveHour = TWELVE_HOUR_MODE;
}

//Configures the microcontroller to not convert from the default 24 hour mode.
void set24Hour()
{
	_isTwelveHour = TWENTYFOUR_HOUR_MODE;
}

//Returns true if the microcontroller has been configured for 12 hour mode
bool is12Hour()
{
	return _isTwelveHour;
}

//Returns true if the microcontroller is in 12 hour mode and the RTC has an hours value greater than or equal to 12 (Noon).
bool isPM()
{
	if (is12Hour())
	{
		return BCDtoDEC(_time[TIME_HOURS]) >= 12;
	}
	else
	{
		return false;
	}
}

//Returns the date in MM/DD/YYYY format.
char* stringDateUSA()
{
	static char date[11]; //Max of mm/dd/yyyy with \0 terminator
	sprintf(date, "%02d/%02d/20%02d", BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_YEAR]));
	return(date);
}

//Returns the date in the DD/MM/YYYY format.
char*  stringDate()
{
	static char date[11]; //Max of dd/mm/yyyy with \0 terminator
	sprintf(date, "%02d/%02d/20%02d", BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_YEAR]));
	return(date);
}

//Returns the time in hh:mm:ss (Adds AM/PM if in 12 hour mode).
char* stringTime()
{
	static char time[11]; //Max of hh:mm:ssXM with \0 terminator

	if(is12Hour() == true)
	{
		char half = 'A';
		uint8_t twelveHourCorrection = 0;
		if(isPM())
		{
			half = 'P';
			if (BCDtoDEC(_time[TIME_HOURS]) > 12)
			{
				twelveHourCorrection = 12;
			}
		}
		sprintf(time, "%02d:%02d:%02d%cM", BCDtoDEC(_time[TIME_HOURS]) - twelveHourCorrection, BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]), half);
	}
	//else
	sprintf(time, "%02d:%02d:%02d", BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]));
	
	return(time);
}

//Returns the most recent timestamp captured on the EVI pin (if the EVI pin has been configured to capture events)
//RV-3032-C7 also captures day, month, and year when EVI pin is triggered. Modify string to include RV3032_DAY_CAPTURE, RV3032_MONTH_CAPTURE, and RV3032_YEAR_CAPTURE if desired
char* stringTimestamp()
{
	static char time[14]; //Max of hh:mm:ss:HHXM with \0 terminator

	if(is12Hour() == true)
	{
		char half = 'A';
		uint8_t twelveHourCorrection = 0;
		if(isPM())
		{
			half = 'P';
			if (BCDtoDEC(_time[TIME_HOURS]) > 12)
			{
				twelveHourCorrection = 12;
			}
		}
		sprintf(time, "%02d:%02d:%02d:%02d%cM", BCDtoDEC(readRegister(RV3032_HOURS_CAPTURE)) - twelveHourCorrection, BCDtoDEC(readRegister(RV3032_MINUTES_CAPTURE)),  BCDtoDEC(readRegister(RV3032_SECONDS_CAPTURE)), BCDtoDEC(readRegister(RV3032_HUNDREDTHS_CAPTURE)), half);
	}
	else
	sprintf(time, "%02d:%02d:%02d:%02d", BCDtoDEC(readRegister(RV3032_HOURS_CAPTURE)), BCDtoDEC(readRegister(RV3032_MINUTES_CAPTURE)), BCDtoDEC(readRegister(RV3032_SECONDS_CAPTURE)), BCDtoDEC(readRegister(RV3032_HUNDREDTHS_CAPTURE)));
	
	return(time);
}

//Returns timestamp in ISO 8601 format (yyyy-mm-ddThh:mm:ss).
char* stringTime8601()
{
	static char timeStamp[21]; //Max of yyyy-mm-ddThh:mm:ss with \0 terminator

	sprintf(timeStamp, "20%02d-%02d-%02dT%02d:%02d:%02d", BCDtoDEC(_time[TIME_YEAR]), BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]));
	
	return(timeStamp);
}

//Returns time in UNIX Epoch time format
uint32_t getEpoch()
{
	struct tm tm;

	tm.tm_isdst = -1;
	tm.tm_yday = 0;
	tm.tm_wday = 0;
	tm.tm_year = BCDtoDEC(_time[TIME_YEAR]) + 100;
	tm.tm_mon = BCDtoDEC(_time[TIME_MONTH]) - 1;
	tm.tm_mday = BCDtoDEC(_time[TIME_DATE]);
	tm.tm_hour = BCDtoDEC(_time[TIME_HOURS]);
	tm.tm_min = BCDtoDEC(_time[TIME_MINUTES]);
	tm.tm_sec = BCDtoDEC(_time[TIME_SECONDS]);

  return mktime(&tm);
}

//Sets time using UNIX Epoch time
bool setEpoch(uint32_t value)
{
	if (value < 946684800) {
		value = 946684800; // 2000-01-01 00:00:00
	}

	//struct tm tm;
	time_t t = value;
	struct tm* tmp = gmtime(&t);

	_time[TIME_SECONDS] = DECtoBCD(tmp->tm_sec);
	_time[TIME_MINUTES] = DECtoBCD(tmp->tm_min);
	_time[TIME_HOURS] = DECtoBCD(tmp->tm_hour);
	_time[TIME_DATE] = DECtoBCD(tmp->tm_mday);
	_time[TIME_WEEKDAY] = 1 << tmp->tm_wday;
	_time[TIME_MONTH] = DECtoBCD(tmp->tm_mon + 1);
	_time[TIME_YEAR] = DECtoBCD(tmp->tm_year - 100);
		
	return setTime(_time, TIME_ARRAY_LENGTH); //Subtract one as we don't write to the hundredths register
}

//Set time and date/day registers of RV3032
bool setTimeF(uint8_t sec, uint8_t min, uint8_t hour, uint8_t weekday, uint8_t date, uint8_t month, uint16_t year)
{
	_time[TIME_SECONDS] = DECtoBCD(sec);
	_time[TIME_MINUTES] = DECtoBCD(min);
	_time[TIME_HOURS] = DECtoBCD(hour);
	_time[TIME_DATE] = DECtoBCD(date);
	_time[TIME_WEEKDAY] = 1 << weekday;
	_time[TIME_MONTH] = DECtoBCD(month);
	_time[TIME_YEAR] = DECtoBCD(year - 2000);
		
	return setTime(_time, TIME_ARRAY_LENGTH); //Subtract one as we don't write to the hundredths register
}

//Set time and date/day registers of RV3032 (using data array)
bool setTime(uint8_t * time, uint8_t len)
{
	if (len != TIME_ARRAY_LENGTH)
		return false;
	
	return writeMultipleRegisters(RV3032_SECONDS, time + 1, len - 1); //We use length - 1 as that is the length without the read-only hundredths register. We also point to the second element in the time array as hundredths is read only
}

bool setHundredthsToZero()
{
	bool temp = write1Bit(RV3032_CONTROL2, CONTROL2_STOP, ENABLE);
	temp &= write1Bit(RV3032_CONTROL2, CONTROL2_STOP, DISABLE);
	return temp;
}

bool setSeconds(uint8_t value)
{
	_time[TIME_SECONDS] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool setMinutes(uint8_t value)
{
	_time[TIME_MINUTES] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool setHours(uint8_t value)
{
	_time[TIME_HOURS] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool setDate(uint8_t value)
{
	_time[TIME_DATE] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool setMonth(uint8_t value)
{
	_time[TIME_MONTH] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool setYear(uint16_t value)
{
	_time[TIME_YEAR] = DECtoBCD(value - 2000);
	return setTime(_time, TIME_ARRAY_LENGTH);
}


bool setWeekday(uint8_t value) //value is anywhere between 0=sunday and 6=saturday
{
	if (value > 6)
	{
		value = 6;
	}
	_time[TIME_WEEKDAY] = 1 << value;
	return setTime(_time, TIME_ARRAY_LENGTH);
}

//Move the hours, mins, sec, etc registers from RV-3032 into the _time array
//Needs to be called before printing time or date
//We do not protect the GPx registers. They will be overwritten. The user has plenty of RAM if they need it.
bool updateTime()
{
	if (readMultipleRegisters(RV3032_HUNDREDTHS, _time, TIME_ARRAY_LENGTH) == false)
		return(false); //Something went wrong

	if (BCDtoDEC(_time[TIME_SECONDS]) == 59) //If seconds are at 59, read again to make sure we didn't accidentally skip a minute
	{	
		uint8_t tempTime[TIME_ARRAY_LENGTH];
		if (readMultipleRegisters(RV3032_HUNDREDTHS, tempTime, TIME_ARRAY_LENGTH) == false)
		{
			return(false); //Something went wrong
		}
		if (BCDtoDEC(tempTime[TIME_SECONDS]) == 0) //If the reading for seconds changed, then our new data is correct, otherwise, we can leave the old data.
		{
			memcpy(_time, tempTime, TIME_ARRAY_LENGTH);
		}
	}
	return true;
}

uint8_t getHundredths()
{
	return BCDtoDEC(_time[TIME_HUNDREDTHS]);
}

uint8_t getSeconds()
{
	return BCDtoDEC(_time[TIME_SECONDS]);
}

uint8_t getMinutes()
{
	return BCDtoDEC(_time[TIME_MINUTES]);
}

uint8_t getHours()
{
	uint8_t tempHours = BCDtoDEC(_time[TIME_HOURS]);
	if (is12Hour())
	{
		if (tempHours > 12)
		{
			tempHours -= 12;
		}
	}
	return tempHours;
}

uint8_t getDate()
{
	return BCDtoDEC(_time[TIME_DATE]);
}

uint8_t getWeekday()
{
	uint8_t tempWeekday = _time[TIME_WEEKDAY];
	//tempWeekday = log(tempWeekday) / log(2);
	return tempWeekday;
}

uint8_t getMonth()
{
	return BCDtoDEC(_time[TIME_MONTH]);
}

uint16_t getYear()
{
	return BCDtoDEC(_time[TIME_YEAR]) + 2000;
}

uint8_t getHundredthsCapture()
{
	return BCDtoDEC(readRegister(RV3032_HUNDREDTHS_CAPTURE));
}
int8_t getTemperature()
{
	int8_t temp;
	//float temp2;
	temp = readRegister(RV3032_TEMP_MSB);
	//temp = temp << 4;
	//temp = temp + readRegister(RV3032_TEMP_LSB)+1;
	//temp2 = temp2/float(16);
	return temp;
}

uint8_t getSecondsCapture()
{
	return BCDtoDEC(readRegister(RV3032_SECONDS_CAPTURE));
}

uint8_t getMinutesCapture()
{
  return BCDtoDEC(readRegister(RV3032_MINUTES_CAPTURE));
}

//Takes the time from the last build and uses it as the current time
//Works very well as an arduino sketch
bool setToCompilerTime()
{
	_time[TIME_SECONDS] = DECtoBCD(BUILD_SECOND);
	_time[TIME_MINUTES] = DECtoBCD(BUILD_MINUTE);
	_time[TIME_HOURS] = DECtoBCD(BUILD_HOUR);
	
	_time[TIME_MONTH] = DECtoBCD(BUILD_MONTH);
	_time[TIME_DATE] = DECtoBCD(BUILD_DATE);
	_time[TIME_YEAR] = DECtoBCD(BUILD_YEAR - 2000); //! Not Y2K (or Y2.1K)-proof :(

	// Calculate weekday (from here: http://stackoverflow.com/a/21235587)
	// 0 = Sunday, 6 = Saturday
	uint16_t d = BUILD_DATE;
	uint16_t m = BUILD_MONTH;
	uint16_t y = BUILD_YEAR;
	uint16_t weekday = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 + 1;
	_time[TIME_WEEKDAY] = 1 << weekday;
	
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool setCalibrationOffset(float ppm)
{
  uint8_t integerOffset = readRegister(RV3032_EEPROM_OFFSET);
	
	integerOffset = ppm / 0.2384; //.2384 is ppm/LSB
	if (integerOffset < 0)
	{
		integerOffset += 64;
	}
 
	return writeRegister(RV3032_EEPROM_OFFSET, integerOffset);
}

float  getCalibrationOffset()
{
	int8_t value = readRegister(RV3032_EEPROM_OFFSET);
  value &= (0b00111111);
	if (value > 32)
	{
		value -= 64;
	}
	return value * .2384;
}


uint8_t  getEepromPMU()
{
	return readRegister(RV3032_EEPROM_PMU);
}
uint8_t  getEepromOffset()
{
	return readRegister(RV3032_EEPROM_OFFSET);
}
uint8_t  getEepromClkout_1()
{
	return readRegister(RV3032_EEPROM_CLKOUT_1);
}
uint8_t  getEepromClkout_2()
{
	return readRegister(RV3032_EEPROM_CLKOUT_2);
}
uint8_t  getEepromTreference_0()
{
	return readRegister(RV3032_EEPROM_TREFERENCE_0);
}
uint8_t  getEepromTreference_1()
{
	return readRegister(RV3032_EEPROM_TREFERENCE_1);
}

bool setNCLKE(bool value)
{
	return write1Bit(RV3032_EEPROM_PMU, PMU_NCLKE, value);
}
bool setBSM(uint8_t value)
{
	return write2Bit(RV3032_EEPROM_PMU, PMU_BSM, value);
}




bool setEVICalibration(bool eviCalibration)
{
  return write1Bit(RV3032_CONTROL2, CONTROL2_STOP, eviCalibration);
}

bool setEVIDebounceTime(uint8_t debounceTime)
{
	return write2Bit(RV3032_EVI_CONTROL, EVI_CONTROL_ET, debounceTime);
}

bool setEVIEdgeDetection(bool edge)
{
	return write1Bit(RV3032_EVI_CONTROL, EVI_CONTROL_EHL, edge);
}

bool setTSOverwrite(bool overwrite)
{
  return write1Bit(RV3032_TS_CONTROL, TS_CONTROL_EVOW, overwrite);
}

uint8_t getEVIDebounceTime()
{
	return readTwoBits(RV3032_EVI_CONTROL, EVI_CONTROL_ET);
}

bool getEVICalibration()
{
	return readBit(RV3032_EVI_CONTROL, EVI_CONTROL_ESYN);
}

bool getEVIEdgeDetection()
{
	return readBit(RV3032_EVI_CONTROL, EVI_CONTROL_EHL);
}

bool setCountdownTimerEnable(bool timerState)
{
	return write1Bit(RV3032_CONTROL1, CONTROL1_TE, timerState);
}

bool setCountdownTimerFrequency(uint8_t countdownTimerFrequency)
{
	return write2Bit(RV3032_CONTROL1, CONTROL1_TD, countdownTimerFrequency);
}

bool setCountdownTimerClockTicks(uint16_t clockTicks)
{
	//First handle the upper bit, as we need to preserve the GPX bits
	uint8_t value = readRegister(RV3032_TIMER_1);
	value &= (0<<8); //Clear the least significant nibble
	value |= (clockTicks >> 8);
	bool returnValue = writeRegister(RV3032_TIMER_1, value);
	value = clockTicks & 0x00FF;
	returnValue &= writeRegister(RV3032_TIMER_0, value);
	return returnValue;
}

bool setClockOutTimerFrequency(uint8_t clockOutTimerFrequency)
{
  write1Bit(RV3032_EEPROM_CLKOUT_2, EEPROM_CLKOUT2_FD, DISABLE);
	return write2Bit(RV3032_EEPROM_CLKOUT_2, EEPROM_CLKOUT2_FD, clockOutTimerFrequency);
}

bool getCountdownTimerEnable()
{
	return readBit(RV3032_CONTROL1, CONTROL1_TE);
}

uint8_t getCountdownTimerFrequency()
{
	return readTwoBits(RV3032_CONTROL1, CONTROL1_TD);
}

uint16_t getCountdownTimerClockTicks()
{
	uint16_t value = readRegister(RV3032_TIMER_1) << 8;
	value |= readRegister(RV3032_TIMER_0);
	return value;
}

uint8_t getClockOutTimerFrequency()
{
	return readTwoBits(RV3032_EEPROM_CLKOUT_2, EEPROM_CLKOUT2_FD);
}

bool setPeriodicTimeUpdateFrequency(bool timeUpdateFrequency)
{
		return write1Bit(RV3032_CONTROL1, CONTROL1_USEL, timeUpdateFrequency);
}

bool getPeriodicTimeUpdateFrequency()
{	
	return readBit(RV3032_CONTROL1, CONTROL1_USEL);
}

/********************************
Set Alarm Mode controls which parts of the time have to match for the alarm to trigger.
When the RTC matches a given time, make an interrupt fire.
Setting a bit to 1 means that the RTC does not check if that value matches to trigger the alarm
********************************/
void setItemsToMatchForAlarm(bool minuteAlarm, bool hourAlarm, bool dateAlarm)
{
	write1Bit(RV3032_MINUTES_ALARM, ALARM_ENABLE, !minuteAlarm); //For some reason these bits are active low
	write1Bit(RV3032_HOURS_ALARM, ALARM_ENABLE, !hourAlarm);
	write1Bit(RV3032_DATE_ALARM, ALARM_ENABLE, !dateAlarm);
}

bool setAlarmMinutes(uint8_t minute)
{
	uint8_t value = readRegister(RV3032_MINUTES_ALARM);
	value &= (1 << ALARM_ENABLE); //clear everything but enable bit
	value |= DECtoBCD(minute);
	return writeRegister(RV3032_MINUTES_ALARM, value);
}

bool setAlarmHours(uint8_t hour)
{
	uint8_t value = readRegister(RV3032_HOURS_ALARM);
	value &= (1 << ALARM_ENABLE); //clear everything but enable bit
	value |= DECtoBCD(hour);
	return writeRegister(RV3032_HOURS_ALARM, value);
}

bool setAlarmDate(uint8_t date)
{
	uint8_t value = readRegister(RV3032_DATE_ALARM);
	value &= (1 << ALARM_ENABLE); //clear everything but enable bit
	value |= DECtoBCD(date);
	return writeRegister(RV3032_DATE_ALARM, value);
}


uint8_t getAlarmMinutes()
{
	return BCDtoDEC(readRegister(RV3032_MINUTES_ALARM));
}


uint8_t getAlarmHours()
{
	return BCDtoDEC(readRegister(RV3032_HOURS_ALARM));
}

uint8_t getAlarmDate()
{
	return BCDtoDEC(readRegister(RV3032_DATE_ALARM));
}

/*********************************
Given a bit location, enable the interrupt
INTERRUPT_CLKIE 6
INTERRUPT_UIE  	5
INTERRUPT_TIE	  4
INTERRUPT_AIE  	3
INTERRUPT_EIE 	2
*********************************/
bool enableHardwareInterrupt(uint8_t source)
{
	uint8_t value = readRegister(RV3032_CONTROL2);
	value |= (1<<source); //Set the interrupt enable bit
	return writeRegister(RV3032_CONTROL2, value);
}

bool disableHardwareInterrupt(uint8_t source)
{
	uint8_t value = readRegister(RV3032_CONTROL2);
	value &= ~(1 << source); //Clear the interrupt enable bit
	return writeRegister(RV3032_CONTROL2, value);
}

bool disableAllInterrupts()
{
	uint8_t value = readRegister(RV3032_CONTROL2);
	value &= 1; //Clear all bits except for Reset
	return writeRegister(RV3032_CONTROL2, value);
}

bool getInterruptFlag(uint8_t flagToGet)
{
	uint8_t flag = readRegister(RV3032_STATUS);
	flag &= (1 << flagToGet);
	flag = flag >> flagToGet;
	return flag;
}

bool clearAllInterruptFlags() //Read the status register to clear the current interrupt flags
{
	return writeRegister(RV3032_STATUS, 0b00000000);//Write all 0's to clear all flags
}

bool clearInterruptFlag(uint8_t flagToClear)
{
	uint8_t value = readRegister(RV3032_STATUS);
	value &= ~(1 << flagToClear); //clear flag
	return writeRegister(RV3032_STATUS, value);
}

uint8_t BCDtoDEC(uint8_t val)
{
	return ( ( val / 0x10) * 10 ) + ( val % 0x10 );
}

// BCDtoDEC -- convert decimal to binary-coded decimal (BCD)
uint8_t DECtoBCD(uint8_t val)
{
	return ( ( val / 10 ) * 0x10 ) + ( val % 10 );
}

bool readBit(uint8_t regAddr, uint8_t bitAddr)
{
	return ((readRegister(regAddr) & (1 << bitAddr)) >> bitAddr);
}

uint8_t readTwoBits(uint8_t regAddr, uint8_t bitAddr)
{
	return ((readRegister(regAddr) & (3 << bitAddr)) >> bitAddr);
}

bool write1Bit(uint8_t regAddr, uint8_t bitAddr, bool bitToWrite)
{
	uint8_t value = readRegister(regAddr);
	value &= ~(1 << bitAddr);
	value |= bitToWrite << bitAddr;
	return writeRegister(regAddr, value);
}

bool write2Bit(uint8_t regAddr, uint8_t bitAddr, uint8_t bitToWrite) //If we see an unsigned 8-bit, we know we have to write two bits.
{
	uint8_t value = readRegister(regAddr);
	value &= ~(3 << bitAddr);
	value |= bitToWrite << bitAddr;
	return writeRegister(regAddr, value);
}

bool checkI2Cerror(uint8_t v)
{
	if(v == HAL_OK) return false;

	printf("i2c error\n");
	return true;
}

uint8_t readRegister(uint8_t addr)
{
	uint8_t pData[12],ret;
	pData[0]=addr;

	ret = HAL_I2C_Master_Transmit(&hi2c1, RV3032_ADDR<<1, pData, 1, 1);
	if(checkI2Cerror(ret)) return false;
	ret = HAL_I2C_Master_Receive (&hi2c1, RV3032_ADDR<<1, pData, 2, 1);
	if(checkI2Cerror(ret)) return false;

	return *pData;
}

bool writeRegister(uint8_t addr, uint8_t val)
{
	uint8_t pData[12],ret;
	uint8_t pValue[12];

	pData[0]=addr;
	pValue[0]=val;

	ret = HAL_I2C_Master_Transmit(&hi2c1, RV3032_ADDR<<1, pData, 1, 1);
	if(checkI2Cerror(ret)) return false;
	ret = HAL_I2C_Master_Transmit(&hi2c1, RV3032_ADDR<<1, pValue, 1, 1);
	if(checkI2Cerror(ret)) return false;

	return(true);
}

bool writeMultipleRegisters(uint8_t addr, uint8_t * values, uint8_t len)
{
	uint8_t pData[12],ret;
	pData[0]=addr;

	ret = HAL_I2C_Master_Transmit(&hi2c1, RV3032_ADDR<<1, pData, 1, 1);
	if(checkI2Cerror(ret)) return false;

	for (uint8_t i = 0; i < len; i++)
	{
		ret = HAL_I2C_Master_Transmit(&hi2c1, RV3032_ADDR<<1, values+i, 1, 1);
		if(checkI2Cerror(ret)) return false;
	}
	return(true);
}

bool readMultipleRegisters(uint8_t addr, uint8_t * dest, uint8_t len)
{
	uint8_t pData[12],ret;
	pData[0]=addr;

	ret = HAL_I2C_Master_Transmit(&hi2c1, RV3032_ADDR<<1, pData, 1, 1);
	if(checkI2Cerror(ret)) return false;

	for (uint8_t i = 0; i < len; i++)
	{
		ret = HAL_I2C_Master_Receive(&hi2c1, RV3032_ADDR<<1, dest+i, 1, 1);
		if(checkI2Cerror(ret)) return false;
	}
	return(true);
}
