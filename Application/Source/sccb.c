#include "stm32f4xx_hal.h" 
#include "sccb.h"
#include "delay.h"
///////////////////////////////////////////////////////////////////////////////	 
/*SCCB handle *////////////////////////////////////////////////////////////////								  
///////////////////////////////////////////////////////////////////////////////

#define SCCB_DELAY_US   50

//ini SDA as output
void SCCB_Init(void)
{			
    
    SCCB_SDA_OUT();	   
}			 

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    SCCB_SDA_HIGH;     //�����߸ߵ�ƽ	   
    SCCB_SCL_HIGH;	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(SCCB_DELAY_US);  
    SCCB_SDA_LOW;
    delay_us(SCCB_DELAY_US);  	 
    SCCB_SCL_LOW;	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    SCCB_SDA_LOW;
    delay_us(SCCB_DELAY_US);  	 
    SCCB_SCL_HIGH;	
    delay_us(SCCB_DELAY_US);   
    SCCB_SDA_HIGH;
    delay_us(SCCB_DELAY_US);  
}  
//����NA�ź�
void SCCB_No_Ack(void)
{
    delay_us(SCCB_DELAY_US);  
    SCCB_SDA_HIGH;
    SCCB_SCL_HIGH;	
    delay_us(SCCB_DELAY_US);  
    SCCB_SCL_LOW;	
    delay_us(SCCB_DELAY_US);  
    SCCB_SDA_LOW;	
    delay_us(SCCB_DELAY_US);  
}
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
uint8_t SCCB_WR_Byte(uint8_t dat)
{
    uint8_t j,res;	 
    for(j=0;j<8;j++) //ѭ��8�η�������
    {
        if(dat&0x80)SCCB_SDA_HIGH;
        else SCCB_SDA_LOW;
        dat<<=1;
        delay_us(SCCB_DELAY_US);  
        SCCB_SCL_HIGH;	
        delay_us(SCCB_DELAY_US);  
        SCCB_SCL_LOW;		   
    }			 
    SCCB_SDA_IN();		//����SDAΪ���� 
    delay_us(SCCB_DELAY_US);  
    SCCB_SCL_HIGH;			//���յھ�λ,���ж��Ƿ��ͳɹ�
    delay_us(SCCB_DELAY_US);  
    if(SCCB_READ_SDA)res=1;  //SDA=1����ʧ�ܣ�����1
    else res=0;         //SDA=0���ͳɹ�������0
    SCCB_SCL_LOW;		 
    SCCB_SDA_OUT();		//����SDAΪ���    
    return res;  
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
uint8_t SCCB_RD_Byte(void)
{
    uint8_t temp=0,j;    
    SCCB_SDA_IN();		//����SDAΪ����  
    for(j=8;j>0;j--) 	//ѭ��8�ν�������
    {		     	  
        delay_us(SCCB_DELAY_US);  
        SCCB_SCL_HIGH;
        temp=temp<<1;
        if(SCCB_READ_SDA)
            temp++;   
        delay_us(SCCB_DELAY_US);  
        SCCB_SCL_LOW;
    }	
    SCCB_SDA_OUT();		//����SDAΪ���    
    return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
    uint8_t res=0;
    SCCB_Start(); 					//����SCCB����
    if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
    delay_us(SCCB_DELAY_US);  
    if(SCCB_WR_Byte(reg))
        res=1;		//д�Ĵ�����ַ	  
    delay_us(SCCB_DELAY_US);  
    if(SCCB_WR_Byte(data))
        res=1; 	//д����	 
    SCCB_Stop();	
    if(res==1)
    {
        res=1;
    }
    return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
uint8_t SCCB_RD_Reg(uint8_t reg)
{
    uint8_t val=0;
    SCCB_Start(); 				//����SCCB����
    SCCB_WR_Byte(SCCB_ID);		//д����ID	  
    delay_us(SCCB_DELAY_US);
    SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
    delay_us(SCCB_DELAY_US);  	  
    SCCB_Stop();   
    delay_us(SCCB_DELAY_US);  	   
    //���üĴ�����ַ�󣬲��Ƕ�
    SCCB_Start();
    SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����	  
    delay_us(SCCB_DELAY_US);  
    val=SCCB_RD_Byte();		 	//��ȡ����
    SCCB_No_Ack();
    SCCB_Stop();
    return val;
}


void GPIO_out(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void GPIO_in(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}











