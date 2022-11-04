/******************************************************************************
RV3032.h
Humberto Kramm
Novembro 27, 2022
******************************************************************************/

#include "typedefs.h"
#include <stdio.h>
#include <string.h>
#include "i2c.h"

#ifndef RV3032_H
#define RV3032_H

//The 7-bit I2C address of the RV3032
#define RV3032_ADDR							0x51

#define SUNDAY 0x01
#define MONDAY 0x02
#define TUESDAY 0x04
#define WEDNESDAY 0x08
#define THURSDAY 0x10
#define FRIDAY 0x20
#define SATURDAY 0x40

//Register names:
#define RV3032_HUNDREDTHS            0x00
#define RV3032_SECONDS               0x01
#define RV3032_MINUTES               0x02
#define RV3032_HOURS                 0x03
#define RV3032_WEEKDAYS              0x04
#define RV3032_DATE                  0x05
#define RV3032_MONTHS                0x06
#define RV3032_YEARS                 0x07
#define RV3032_MINUTES_ALARM         0x08
#define RV3032_HOURS_ALARM           0x09
#define RV3032_DATE_ALARM            0x0A
#define RV3032_TIMER_0               0x0B
#define RV3032_TIMER_1               0x0C
#define RV3032_STATUS                0x0D
#define RV3032_TEMP_LSB              0x0E
#define RV3032_TEMP_MSB              0x0F
#define RV3032_CONTROL1              0x10
#define RV3032_CONTROL2              0x11
#define RV3032_CONTROL3              0x12
#define RV3032_TS_CONTROL            0x13
#define RV3032_CLOCK_INT_MASK        0x14
#define RV3032_EVI_CONTROL           0x15
#define RV3032_HUNDREDTHS_CAPTURE    0x27
#define RV3032_SECONDS_CAPTURE       0x28
#define RV3032_MINUTES_CAPTURE       0x29
#define RV3032_HOURS_CAPTURE         0x2A
#define RV3032_DAY_CAPTURE           0x2B //Not used, can configure EVI Timestamps to display day, month, and year if desired
#define RV3032_MONTH_CAPTURE         0x2C
#define RV3032_YEAR_CAPTURE          0x2D

#define RV3032_EEPROM_PMU            0xC0
#define RV3032_EEPROM_OFFSET         0xC1
#define RV3032_EEPROM_CLKOUT_1       0xC2 //Used for HF mode CLKOUT readings, default is XTAL mode
#define RV3032_EEPROM_CLKOUT_2       0xC3
#define RV3032_EEPROM_TREFERENCE_0   0xC4
#define RV3032_EEPROM_TREFERENCE_1   0xC5

//Enable Bits for Alarm Registers
#define ALARM_ENABLE						7

////Extension Register Bits
//#define EXTENSION_TEST				7
//#define EXTENSION_WADA				6
//#define EXTENSION_USEL				5
//#define EXTENSION_TE					4
//#define EXTENSION_FD					2
//#define EXTENSION_TD					0

////Flag Register Bits
//#define FLAG_UPDATE						5
//#define FLAG_TIMER						4
//#define FLAG_ALARM					  3
//#define FLAG_EVI							2
//#define FLAG_V2F							1
//#define FLAG_V1F							0

//Status Register Bits (1 is triggered, remember to reset!)
#define STATUS_THF              7 // Temp. High Flag
#define STATUS_TLF              6 // Temp Low Flag
#define STATUS_UF               5 // Periodic Time Update Flag
#define STATUS_TF               4 // Periodic Countdown Update Flag
#define STATUS_AF               3 // Alarm Flag
#define STATUS_EVF              2 // External Event Flag
#define STATUS_PORF             1 // Power On Reset Flag
#define STATUS_VLF              0 // Voltage Low Flag

//Control 1 Register Bits
#define CONTROL1_USEL           4 // Update Interrupt Select
#define CONTROL1_TE             3 // Periodic Countdown Timer Enable
#define CONTROL1_EERD           2 // EEPROM Memory Refresh Disable
#define CONTROL1_TD             0 // Timer Clock Frequency selection

//Control 2 Register Bits
#define CONTROL2_CLKIE          6 // Interrupt Controlled Clock Output Enable
#define CONTROL2_UIE            5 // Periodic Time Update Interrupt Enable
#define CONTROL2_TIE            4 // Periodic Countdown Timer Interrupt Enable
#define CONTROL2_AIE            3 // Alarm Interrupt Enable
#define CONTROL2_EIE            2 // External Event Interrupt Enable bit
#define CONTROL2_STOP           0 // Stop, used for synchronization

//Control 3 Register Bits
#define CONTROL3_BSIE           4 // Backup Switchover Interrupt Enable
#define CONTROL3_THE            3 // Temperature High Enable
#define CONTROL3_TLE            2 // Temperature Low Enable
#define CONTROL3_THIE           1 // Temperature High Interrupt Enable
#define CONTROL3_TLIE           0 // Temperature Low Interrupt Enable

//TS Control Register Bits
#define TS_CONTROL_EVR          7 // Time Stamp EVI Reset
#define TS_CONTROL_EVOW         2 // Time Stamp EVI Overwrite

//EVI Control Register Bits
#define EVI_CONTROL_EHL         6 // Event High/Low Level (Rising/Falling Edge) selection
#define EVI_CONTROL_ET          4 // Event Filtering Time set
#define EVI_CONTROL_ESYN        0 // Event Filtering Time set

//EEPROM CLKOUT Register Bits
#define EEPROM_CLKOUT2_OS       7 // Oscillator Selection
#define EEPROM_CLKOUT2_FD       5 // CLKOUT Frequency Selection in XTAL mode


//Possible Settings
#define TWELVE_HOUR_MODE					         true
#define TWENTYFOUR_HOUR_MODE				       false
#define COUNTDOWN_TIMER_FREQUENCY_4096_HZ	 0b00
#define COUNTDOWN_TIMER_FREQUENCY_64_HZ		 0b01
#define COUNTDOWN_TIMER_FREQUENCY_1_HZ		 0b10
#define COUNTDOWN_TIMER_FREQUENCY_1_60_HZ	 0b11
#define CLKOUT_FREQUENCY_32768_HZ	      	 0b00
#define CLKOUT_FREQUENCY_1024_HZ		     	 0b01
#define CLKOUT_FREQUENCY_64_HZ		      	 0b10
#define CLKOUT_FREQUENCY_1_HZ              0b11

#define COUNTDOWN_TIMER_ON				       	 true
#define COUNTDOWN_TIMER_OFF					       false
#define TIME_UPDATE_1_SECOND				       false
#define TIME_UPDATE_1_MINUTE				       true

#define ENABLE_EVI_CALIBRATION				     true
#define DISABLE_EVI_CALIBRATION				     false
#define EVI_DEBOUNCE_NONE					         0b00 // 0ms, default
#define EVI_DEBOUNCE_256HZ					       0b01 // 256Hz ~= 3.9ms
#define EVI_DEBOUNCE_64HZ					         0b10 // 64Hz ~= 15.6ms
#define EVI_DEBOUNCE_8HZ					         0b11 // 8Hz ~= 125ms
#define RISING_EDGE							           true
#define FALLING_EDGE						           false
#define EVI_CAPTURE_ENABLE			        	 true
#define EVI_CAPTURE_DISABLE					       false

#define ENABLE								             true
#define DISABLE								             false

#define TIME_ARRAY_LENGTH                  8 // Total number of writable values in device

enum time_order {
	TIME_HUNDREDTHS,	// 0
	TIME_SECONDS,		// 1
	TIME_MINUTES,		// 2
	TIME_HOURS,			// 3
	TIME_WEEKDAY,		// 4
	TIME_DATE,			// 5
	TIME_MONTH,			// 6
	TIME_YEAR,			// 7
};

	//bool begin(TwoWire &wirePort = Wire);

	void set12Hour();
	void set24Hour();
	bool is12Hour(); //Returns true if 12hour bit is set
	bool isPM(); //Returns true if is12Hour and PM bit is set

	char* stringDateUSA(); //Return date in mm-dd-yyyy
	char* stringDate(); //Return date in dd-mm-yyyy
	char* stringTime(); //Return time hh:mm:ss with AM/PM if in 12 hour mode
	char* stringTimestamp(); //Return timestamp in hh:mm:ss:hh, note that this must be read the same minute that the timestamp occurs or the minute will be wrong
	char* stringTime8601(); //Return time in ISO 8601 format yyyy-mm-ddThh:mm:ss

	bool setTimeF(uint8_t sec, uint8_t min, uint8_t hour, uint8_t weekday, uint8_t date, uint8_t month, uint16_t year);
	bool setTime(uint8_t * time, uint8_t len);
	bool setEpoch(uint32_t value);
	bool setHundredthsToZero();
	bool setSeconds(uint8_t value);
	bool setMinutes(uint8_t value);
	bool setHours(uint8_t value);
	bool setDate(uint8_t value);
	bool setWeekday(uint8_t value);
	bool setMonth(uint8_t value);
	bool setYear(uint16_t value);

	bool updateTime(); //Update the local array with the RTC registers

	uint8_t getHundredths();
	uint8_t getSeconds();
	uint8_t getMinutes();
	uint8_t getHours();
	uint8_t getDate();
	uint8_t getWeekday();
	uint8_t getMonth();
	uint16_t getYear();	
	uint32_t getEpoch();
	
	uint8_t getHundredthsCapture();
	int8_t getTemperature();
	uint8_t getSecondsCapture();
	uint8_t getMinutesCapture();
	
	bool setToCompilerTime(); //Uses the hours, mins, etc from compile time to set RTC
	
	bool setCalibrationOffset(float ppm);
	float getCalibrationOffset();
	
	uint8_t  getEepromPMU();
	uint8_t  getEepromOffset();
	uint8_t  getEepromClkout_1();
	uint8_t  getEepromClkout_2();
	uint8_t  getEepromTreference_0();
	uint8_t  getEepromTreference_1();

	bool setEVICalibration(bool eviCalibration);
	bool setEVIDebounceTime(uint8_t debounceTime);
	bool setEVIEdgeDetection(bool edge);
	bool setEVIEventCapture(bool capture);
	bool setTSOverwrite(bool overwrite);
	
	bool getEVICalibration();
	uint8_t getEVIDebounceTime();
	bool getEVIEdgeDetection();
	
	
	bool setCountdownTimerEnable(bool timerState); //Starts and stops our countdown timer
	bool setCountdownTimerClockTicks(uint16_t clockTicks);
	bool setCountdownTimerFrequency(uint8_t countdownTimerFrequency);
	
	bool setClockOutTimerFrequency(uint8_t clockOutTimerFrequency);
	uint8_t getClockOutTimerFrequency();

	bool getCountdownTimerEnable();
	uint16_t getCountdownTimerClockTicks();
	uint8_t getCountdownTimerFrequency();
	
	bool setPeriodicTimeUpdateFrequency(bool timeUpdateFrequency);
	bool getPeriodicTimeUpdateFrequency();
	
	void setItemsToMatchForAlarm(bool minuteAlarm, bool hourAlarm, bool dateAlarm); //0 to 7, alarm goes off with match of second, minute, hour, etc
	bool setAlarmMinutes(uint8_t minute);
	bool setAlarmHours(uint8_t hour);
	bool setAlarmDate(uint8_t date);
	
	uint8_t getAlarmMinutes();
	uint8_t getAlarmHours();
	uint8_t getAlarmDate();

	bool enableHardwareInterrupt(uint8_t source); //Enables a given interrupt within Interrupt Enable register
	bool disableHardwareInterrupt(uint8_t source); //Disables a given interrupt within Interrupt Enable register
	bool disableAllInterrupts();
	
	bool getInterruptFlag(uint8_t flagToGet);
	bool clearInterruptFlag(uint8_t flagToClear);
	bool clearAllInterruptFlags();

	//Values in RTC are stored in Binary Coded Decimal. These functions convert to/from Decimal
	uint8_t BCDtoDEC(uint8_t val); 
	uint8_t DECtoBCD(uint8_t val);

	bool readBit(uint8_t regAddr, uint8_t bitAddr);
	uint8_t readTwoBits(uint8_t regAddr, uint8_t bitAddr);
	bool write1Bit(uint8_t regAddr, uint8_t bitAddr, bool bitToWrite);
	bool write2Bit(uint8_t regAddr, uint8_t bitAddr, uint8_t bitToWrite);
	bool checkI2Cerror(uint8_t v);
	uint8_t readRegister(uint8_t addr);
	bool writeRegister(uint8_t addr, uint8_t val);
	bool readMultipleRegisters(uint8_t addr, uint8_t * dest, uint8_t len);
	bool writeMultipleRegisters(uint8_t addr, uint8_t * values, uint8_t len);

	//uint8_t _time2[TIME_ARRAY_LENGTH];
	//bool _isTwelveHour = true;
	//TwoWire *_i2cPort;

#endif
