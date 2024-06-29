/*
 * We need to implement these feature in initial version
 * CH583M ( Setup, Button,Switches, LED, RGB LED,NTC, AHT21, Touch)
 * CH583M Print to Console (OK)
 * CH583M Button (OK)
 * CH583M Switches (OK)
 * CH583M LED (OK)
 * CH583M RGB LED (OK)
 * CH583M AHT21
 * CH583M NTC
 * CH583M ADC Touch
 * CH583M Print Address
 * CHV307 Print Address
 * */

#include "CH58x_common.h"
#include "app_i2c.h"
#include "AHT.h"

uint8_t TxBuff[] = "Hello\r\n";
uint8_t RxBuff[100];
uint8_t trigB;

void button_press()
{
    GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU);      // RXD
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD

    while(GPIOA_ReadPortPin(6))
    {
    	GPIOA_SetBits(GPIO_Pin_9);
    }
}
void switch_on()
{
	GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU);      // RXD
	GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD

	if(GPIOA_ReadPortPin(6))
	{
		GPIOA_SetBits(GPIO_Pin_9);
	}
}
void sendFloatOverUART(float value) {
    char buffer[20]; // Adjust the buffer size as needed

    // Convert float to string with two decimal places
    snprintf(buffer, sizeof(buffer), "%.2f", value);

    // Send the string over UART
    UART1_SendString(buffer, strlen(buffer));
}
int main()
{
    uint8_t len;
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD
    UART1_DefInit();
    //print_console();
    //button_press();

    //GPIOA_SetBits(GPIO_Pin_0);
    //GPIOA_ModeCfg(GPIO_Pin_0, GPIO_ModeOut_PP_5mA);

    //UART1_SendString("S-1\n", sizeof("S-1"));
    //mDelaymS(100);
    //UART1_Reset();
    //i2c_app_init(AHTXX_ADDRESS_X38);
    //AHT21_init();

    //mDelaymS(5000);
    AHT21_init();
    while(1)
    {
    	//mDelaymS(5000);
        //GPIOA_SetBits(GPIO_Pin_0);
    	//UART1_SendString("Start\n", sizeof("Start"));
    	//UART1_Reset();
    	//UART1_SendByte(readHumidity(1));
    	//mDelaymS(100);
    	//UART1_SendString("Temperature: ", strlen("Temperature: "));
//        if(AHT21_init() == 0)
//        {
//        	UART1_SendString("AHT init successfull \n", strlen("AHT init successfull \n"));
//        }
//        else
//        {
//        	UART1_SendString("AHT init is not successfull \n", strlen("AHT init is not successfull \n"));
//        	mDelaymS(500);
//        	return 0;
//        }
        //mDelaymS(100);
        //UART1_SendString("S-2\n", sizeof("S-2"));
        //UART1_Reset();
    	char temp_char[4];
    	uint32_t temp_temperature  = AHT21_Read_Temperature();
    	sprintf(temp_char, "%u", temp_temperature);

    	UART1_SendString("\r\n", strlen("\r\n"));
    	UART1_SendString("Temperature Value is :: ", strlen("Temperature Value is :: "));
    	UART1_SendString(temp_char, strlen(temp_char));
    	UART1_SendString("\r\n", strlen("\r\n"));
    	mDelaymS(1000);

    	temp_char[4];
    	uint32_t temp_humid  = AHT21_Read_Humidity();
    	sprintf(temp_char, "%u", temp_humid);

    	UART1_SendString("\r\n", strlen("\r\n"));
    	UART1_SendString("Humidity Value is :: ", strlen("Humidity Value is :: "));
    	UART1_SendString(temp_char, strlen(temp_char));
    	UART1_SendString("\r\n", strlen("\r\n"));
    	mDelaymS(1000);
    	//sendFloatOverUART(251.2);
    	//sendFloatOverUART(readTemperature(0));
    	//UART1_SendString("\n", strlen("\n"));
        mDelaymS(500);
        //GPIOA_ResetBits(GPIO_Pin_0);
    }
}

/*********************************************************************
 * @fn      UART1_IRQHandler
 * @brief
 * @return  none
 */
//__INTERRUPT
//__HIGH_CODE
//void UART1_IRQHandler(void)
//{
//    volatile uint8_t i;
//    switch(UART1_GetITFlag())
//    {
//        case UART_II_LINE_STAT:
//        {
//            UART1_GetLinSTA();
//            break;
//        }
//        case UART_II_RECV_RDY:
//            for(i = 0; i != trigB; i++)
//            {
//                RxBuff[i] = UART1_RecvByte();
//                UART1_SendByte(RxBuff[i]);
//            }
//            break;
//        case UART_II_RECV_TOUT:
//            i = UART1_RecvString(RxBuff);
//            UART1_SendString(RxBuff, i);
//            break;
//        case UART_II_THR_EMPTY:
//            break;
//        case UART_II_MODEM_CHG:
//            break;
//        default:
//            break;
//    }
//}
