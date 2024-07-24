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
#include "CH58x_adc.h"
#include "app_i2c.h"
#include "AHT.h"
#include "ntc.h"


uint8_t TxBuff[] = "Hello\r\n";
uint8_t RxBuff[100];
uint16_t adcBuff[40];
uint8_t trigB;

volatile uint8_t adclen;
volatile uint8_t DMA_end = 0;

void button_press()
{
    GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU);      // RXD

    while(GPIOA_ReadPortPin(6))
    {
    	GPIOA_SetBits(GPIO_Pin_9);
    }
}
void switch_on()
{
	GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU);      // RXD

	if(GPIOA_ReadPortPin(6))
	{
		GPIOA_SetBits(GPIO_Pin_9);
	}
}

void button_press_led_blink()
{
	//GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);      // RXD
	//GPIOB_SetBits(GPIO_Pin_4);

	//GPIOB_ModeCfg(GPIO_Pin_8, GPIO_ModeOut_PP_20mA);
	//GPIOB_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_20mA);
	//GPIOB_ModeCfg(GPIO_Pin_16, GPIO_ModeOut_PP_20mA);

	//UART1_SendString("\r\n", strlen("\r\n"));
	//UART1_SendString("Hello from button_press_led_blink", strlen("Hello from button_press_led_blink"));
	//UART1_SendString("\r\n", strlen("\r\n"));


	if(!GPIOB_ReadPortPin(GPIO_Pin_4))
	{
		UART1_SendString("\r\n", strlen("\r\n"));
		UART1_SendString("Button read successful", strlen("Button read successful"));
		UART1_SendString("\r\n", strlen("\r\n"));

		GPIOB_SetBits(GPIO_Pin_8);
		mDelaymS(50);

		// Signal blink
//		GPIOB_SetBits(GPIO_Pin_8);
//		GPIOB_ResetBits(GPIO_Pin_9);
//		GPIOB_ResetBits(GPIO_Pin_16);
//		mDelaymS(3000);
//
//		GPIOB_ResetBits(GPIO_Pin_8);
//		GPIOB_SetBits(GPIO_Pin_9);
//		GPIOB_ResetBits(GPIO_Pin_16);
//		mDelaymS(3000);
//
//		GPIOB_ResetBits(GPIO_Pin_8);
//		GPIOB_ResetBits(GPIO_Pin_9);
//		GPIOB_SetBits(GPIO_Pin_16);
//		mDelaymS(3000);

//		GPIOB_SetBits(GPIO_Pin_20);
//		GPIOB_SetBits(GPIO_Pin_21);
	}
	else
	{
		GPIOB_ResetBits(GPIO_Pin_8);
		mDelaymS(50);
	}
}



void test_aht()
{
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
}

void touch_test()
{
	uint8_t      i;
	/* TouchKey采样：选择adc通道 2 做采样，对应 PA12 */
	//PRINT("\n5.TouchKey sampling...\n");
//	GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_Floating);
//	TouchKey_ChSampInit();
//	ADC_ChannelCfg(2);

	for(i = 0; i < 20; i++)
	{
		adcBuff[i] = TouchKey_ExcutSingleConver(0x10, 0); // 连续采样20次
	}
	for(i = 0; i < 20; i++)
	{
		//PRINT("%d \n", adcBuff[i]);
		char temp_char[2];
		sprintf(temp_char, "%u", adcBuff[i]);
		UART1_SendString("\r\n", strlen("\r\n"));
		UART1_SendString("Touch Sampling Value is :: ", strlen("Touch Sampling Value is :: "));
		UART1_SendString(temp_char, strlen(temp_char));
		UART1_SendString("\r\n", strlen("\r\n"));
	}
	ADC_StartUp();
	while(adclen < 20);
	PFIC_DisableIRQ(ADC_IRQn);
//	for(i = 0; i < 20; i++)
//	{
////        PRINT("%d \n", adcBuff[i]);
//		char temp_char[2];
//		sprintf(temp_char, "%u", adcBuff[i]);
//		UART1_SendString("\r\n", strlen("\r\n"));
//		UART1_SendString("Touch Sampling Value is :: ", strlen("Touch Sampling Value is :: "));
//		UART1_SendString(temp_char, strlen(temp_char));
//		UART1_SendString("\r\n", strlen("\r\n"));
//		mDelaymS(500);
//	}
}

void read_ntc_adc()
{
	uint8_t i;

	for(i = 0; i < 20; i++)
	{
		adcBuff[i] = ADC_ExcutSingleConver();
	}
	for(i = 0; i < 20; i++)
	{
		//PRINT("%d \n", adcBuff[i]);
		char temp_char[4];
		uint32_t temp_ntc = adc_to_temperature_celsius(adcBuff[i]);
		//uint32_t temp_ntc = ntc_convertToC(adcBuff[i]);
		sprintf(temp_char, "%u", temp_ntc);
		UART1_SendString("\r\n", strlen("\r\n"));
		UART1_SendString("NTC Value in C degree is :: ", strlen("NTC Value in C degree is :: "));
		UART1_SendString(temp_char, strlen(temp_char));
		UART1_SendString("\r\n", strlen("\r\n"));
	    mDelaymS(1000);
	}

    ADC_StartUp();
    while(adclen < 20);
    PFIC_DisableIRQ(ADC_IRQn);
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
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);      // RXD

	GPIOB_ModeCfg(GPIO_Pin_8, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_20mA);

	GPIOB_ModeCfg(GPIO_Pin_16, GPIO_ModeOut_PP_20mA);
	//GPIOB_ModeCfg(GPIO_Pin_17, GPIO_ModeOut_PP_20mA);

	GPIOB_ModeCfg(GPIO_Pin_20, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(GPIO_Pin_21, GPIO_ModeOut_PP_20mA);

	//GPIOB_SetBits(GPIO_Pin_8);
	//GPIOB_SetBits(GPIO_Pin_9);

	//GPIOB_SetBits(GPIO_Pin_16);
	//GPIOB_SetBits(GPIO_Pin_17);

	//GPIOB_SetBits(GPIO_Pin_20);
	//GPIOB_SetBits(GPIO_Pin_21);

	GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_Floating);

	// touch
	//TouchKey_ChSampInit();
	//ADC_ChannelCfg(2);
	//touch_test();

	// NTC sensor
	//ADC_InterTSSampInit();
	//read_ntc_adc();

    // init AHT sensor
    //AHT21_init();

    while(1)
    {
    	//mDelaymS(1000);
    	//test_aht();
    	button_press_led_blink();
    	//touch_test();
    	//mDelaymS(200);
    	//button_press_led_blink();
    	//mDelaymS(200);

        //GPIOA_ResetBits(GPIO_Pin_0);
    }
}

/*********************************************************************
 * @fn      ADC_IRQHandler
 *
 * @brief   ADC中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void ADC_IRQHandler(void) //adc中断服务程序
{
    if(ADC_GetDMAStatus())
    {
        ADC_StopDMA();
        R16_ADC_DMA_BEG = ((uint32_t)adcBuff) & 0xffff;
        ADC_ClearDMAFlag();
        DMA_end = 1;
    }
    if(ADC_GetITStatus())
    {
        ADC_ClearITFlag();
        if(adclen < 20)
        {
            adcBuff[adclen] = ADC_ReadConverValue();
            ADC_StartUp(); // 作用清除中断标志并开启新一轮采样
        }
        adclen++;
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
