 /******************************************************************************
TCC.h
Humberto Kramm
Novembro 27, 2022
******************************************************************************/

#include "TCC.h"

//****************************************************************************//
//
//  Variables
//
//****************************************************************************//
uint8_t RxLength = 0;
uint8_t aTransmitBuffer[TX_BUFFER_SIZE] = {1,2,3,4};
uint8_t aReceiveBuffer[RX_BUFFER_SIZE] = {0x00};


uint32_t counter;
#define HEADER_MEMORY	0x08080000
#define START_MEMORY	0x08080004
#define END_MEMORY		0x080807CC
#define INDEX_MASK		0x00000FFF
#define BYTES_MEMORY	4
//****************************************************************************//
//
//  Functions
//
//****************************************************************************//
//Executa a transmissão
void TCC_Radio()
{
	HAL_Radio_Init();
	HT_P2P_Init();
}
//Executa a transmissão
void TCC_Transmit(uint32_t data)
{
	aTransmitBuffer[0] = data>>24;
	aTransmitBuffer[1] = data>>16;
	aTransmitBuffer[2] = data>>8;
	aTransmitBuffer[3] = data;

	Set_Transmission();
	do P2P_Process(aTransmitBuffer, TX_BUFFER_SIZE, aReceiveBuffer, RxLength);
	while(checkEndTX());
}

//****************************************************************************//

void _printbits(uint32_t v,uint8_t len)
{
    uint8_t i;
    printf("0b");
    for (i=len ; i>0 ;i--){
        if(v&(1<<(i-1))) printf("1");
        else printf("0");
    }
    printf("\n");
}
//****************************************************************************//
void clearData()
{
	//Salva na memória
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, HEADER_MEMORY, 0);
	HAL_FLASH_Lock();
}
//****************************************************************************//
void saveData(uint32_t payload)
{
	uint32_t index = 0,addr;

	//Get last data saved
	index = *(__IO uint32_t *)HEADER_MEMORY;
	index &= INDEX_MASK;

	//Inc position
	index ++;
	addr = index * BYTES_MEMORY + HEADER_MEMORY;


	//Check overflow memory
	if(addr > END_MEMORY){
		printf("overflow memory\n");
		return;
	}

	//Salva na memória
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, payload);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, HEADER_MEMORY, index);
	HAL_FLASH_Lock();

	printf("%lu\n",index);
}
//****************************************************************************//
void printMemory()
{
	uint32_t index = 0, value,addr;
	int8_t temp;

	//Get last data saved
	index = *(__IO uint32_t *)HEADER_MEMORY;
	index &= INDEX_MASK;

	//Get memory values
	for(int i = 0; i<index ; i++){
		addr = i * BYTES_MEMORY + START_MEMORY;
		value = *(__IO uint32_t *)addr;
		setEpoch(value & 0xFFFFFF00);
		temp = (value & 0xFF) - 128;
		printf("Data: %s\t%s\t%u°C\n",stringDate(),stringTime(),temp);
	}
	printf("Memory: %lu\n",index);
}
//****************************************************************************//
uint16_t readVoltage()
{
	uint16_t voltage;
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc,ADC_CHANNEL_0);
	voltage = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);
	return voltage;
}
//****************************************************************************//




/*
printf("getEepromPMU() "); _printbits(getEepromPMU(),8);
printf("getEepromOffset() "); _printbits(getEepromOffset(),8);
printf("getEepromClkout_1() "); _printbits(getEepromClkout_1(),8);
printf("getEepromClkout_2() "); _printbits(getEepromClkout_2(),8);
printf("getEepromTreference_0() "); _printbits(getEepromTreference_0(),8);
printf("getEepromTreference_1() "); _printbits(getEepromTreference_1(),8);

*/
//if(setNCLKE(1)) printf("setNCLKE()");
//if(setBSM(0b01)) printf("setBSM()");
