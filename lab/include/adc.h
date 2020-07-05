#ifndef ADC_H
#define ADC_H
//ADC
void ADC0_Init(void);                       // ADC0初始化
void ADC0_ISR(void);                        //ADC0中断服务程序
unsigned int ADC_Value(unsigned char Flag); //计算ADC数值
//DAC
void DAC0_Init(void); //DAC0初始化

#endif
