#include <reg52.h>
#include <lcd.h>
#include <i2c.h>
#include <main.h>
#include <rc522.h>
#include <ir.h>
#include <ds1302.h>
#define uchar unsigned char 
#define uint  unsigned int	
#define GPIO_KEY P1    
sbit beep = P1^5;
sbit relay = P3^1;    
unsigned char KeyValue;
unsigned char KeyState;
typedef unsigned char INT8U;
unsigned char dat[]="0123456789S0QU00D00T00A";
uchar dis0[]="    Welcome    ";
uchar dis5[]="  Please Input ";
INT8U KEY_BUF[6]={'0','0','0','0','0','0'};
INT8U Test_BUF[6];
int i,num;

u8 UID[5],Temp[4];
u8 data1[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
u8 data2[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02};
				
u8 data_buff[16] = {0};
u8 code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
extern u8 gIrCode[5];
void relay_ON();
void Delay10ms();   
void KeyDown();
void LcdDisplay();		 
void show();
void beep1(); 
void Passwordinit(){
  for(i = 0;i<6;i++)
  {
    At24c02Write(i+1,'0');
    Delay10ms(); 
  }
}

void main(void)
{
    
    uchar key_count;
    INT8U table[6]="------",
          table2[6]="------";
    u8 status = 0;
	u8 flag = 1;
	relay = 1;
	LcdInit();
	IrInit();
	Ds1302Init();
  //Passwordinit();
    SysInit();
	KeyState=0;

    while(1)
    {
	
	    if (gIrCode[4]==0x45) 
		 {
		    beep1();
			LcdWriteCom(0x01);
			show();
			relay_ON();					 
			for(i=0;i<5;i++)
			gIrCode[i]=0;
		}
        for(i = 0;i<6;i++)
         {
          KEY_BUF[i]=At24c02Read(i+1); 
         }
		 
        KeyDown();
        if(KeyState)
        {
             KeyState=0;
             switch(dat[KeyValue])
            {
             case '1': LcdWriteCom(0x01);  
                       LcdWrite_Row_Data(1,0,dis5);
                       KeyState=0;
                       key_count=0;
                       while(1)
                       { 
                        KeyDown();
                        if(KeyState)
                        {
                           KeyState=0; 

                          if(dat[KeyValue]>= '0' && dat[KeyValue]<='9')
                          {
                            beep1();
                            table[key_count++]=dat[KeyValue];
                            LcdWrite_Row_Data(2,key_count,"*");
                          }
                          if(dat[KeyValue] == 'T')
                          {
                            beep1();
                            table[--key_count]='-';
                            LcdWrite_Row_Data(2,key_count+1," ");
                          }
                           if(key_count==6 && dat[KeyValue] == 'S')
                           {
                           if( table[0]==KEY_BUF[0] &&
                               table[1]==KEY_BUF[1] &&
                               table[2]==KEY_BUF[2] &&
                               table[3]==KEY_BUF[3] &&
                               table[4]==KEY_BUF[4] &&
                               table[5]==KEY_BUF[5]  )
                
                             {   LcdWriteCom(0x01);
                                 show();
								 beep1();
								 relay_ON();
								 
                                 break;  
                             }else{    
                                 beep1();
                                 beep1();
                                 LcdWriteCom(0x01);
                                 LcdWrite_Row_Data(1,0," PassWord ERROR  ");
                                 break;
                                   }  
                             break; 
                            }
                        }
                       }
                       break;
             case '2': LcdWriteCom(0x01);
                       LcdWrite_Row_Data(1,0," Ori PassWord ");
                       KeyState=0;
                       key_count=0;
                       while(1)
                       { 
                        KeyDown();
                        if(KeyState)
                        {
                           KeyState=0;
                            if(dat[KeyValue]>= '0' && dat[KeyValue]<='9')
                          {
                            beep1();
                            table[key_count++]=dat[KeyValue];
                            LcdWrite_Row_Data(2,key_count,"*");
                          }
                          if(dat[KeyValue] == 'T')
                          {
                            beep1();
                            table[--key_count]='-';
                            LcdWrite_Row_Data(2,key_count+1," ");
                          }
                          if(key_count==6 && dat[KeyValue] == 'S')
                           {  
                            if( table[0]==KEY_BUF[0] &&
                                table[1]==KEY_BUF[1] &&
                                table[2]==KEY_BUF[2] &&
                                table[3]==KEY_BUF[3] &&
                                table[4]==KEY_BUF[4] &&
                                table[5]==KEY_BUF[5]  )
                
                               {   //New Password
                                   LcdWriteCom(0x01);
                                   LcdWrite_Row_Data(1,0," New PassWord ");
                                   KeyState=0;
                                   key_count=0;
                                   while(1)
                                   { 
                                    KeyDown();
                                    if(KeyState)
                                     {
                                      KeyState=0;
                                      if(dat[KeyValue]>= '0' && dat[KeyValue]<='9')
                                        {
                                         beep1();
                                         table2[key_count++]=dat[KeyValue];
                                         LcdWrite_Row_Data(2,key_count,"*");
                                        }
                                      if(dat[KeyValue] == 'T')
                                        {
                                         beep1();
                                         table2[--key_count]='-';
                                         LcdWrite_Row_Data(2,key_count+1," ");
                                        }
                                      if(key_count==6 && dat[KeyValue] == 'S')
                                        {
                                         for(i=0;i<6;i++)
                                            KEY_BUF[i]=table2[i];
                                          beep1();
                                          LcdWriteCom(0x01);
                                          LcdWrite_Row_Data(1,0," RESET SUCCESS ");
                                         //writer to eeprom
										 
                                          for(i = 0;i<6;i++)
                                            {
                                              At24c02Write(i+1,KEY_BUF[i]);
                                              Delay10ms(); 
                                            }
										

                                          beep1();
                                         break; 
                                        }
                                     }
                                   }
                         break; }else{
                                      beep1();
                                      LcdWriteCom(0x01);
                                      LcdWrite_Row_Data(1,0," PassWord ERROR  ");
                                      break;
                                      }
                        }
                        }
                        } 
                       break;
             case '3': LcdWriteCom(0x01);
                       LcdWrite_Row_Data(1,0," 1.IC InPut   ");
                       LcdWrite_Row_Data(2,0," 2.Add New Cad");
					   KeyState=0;
					   while(1)
                       {
                        KeyDown();
                        if(KeyState)
                        {
                          KeyState=0;
                          switch(dat[KeyValue]){
                          case '1':LcdWriteCom(0x01);
						           while(flag ==1 )
								{   LcdWrite_Row_Data(1,0," IC CARD:   ");
                                   if (PcdRequest(0x52, Temp) == MI_OK) //Ѱ�����ɹ���Temp����Ϊ����
									{
										if (PcdAnticoll(UID) == MI_OK)//����ͻ��UID��������Ϊ�����к�
										{	 
											status = PcdSelect(UID);//ѡ����Ƭ
											if(status != MI_OK)
												continue;
					
											status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, UID);//��֤��Ƭ����
											if(status != MI_OK)
												continue;
					
											status = PcdRead(1, data_buff);	//���飬����Ǯ����Ľ��
											if(status != MI_OK)
												continue;
								
										if (data1[15]!=data_buff[15] && data2[15]!=data_buff[15]) 
										     {
											   beep1();
											   beep1();
											   for(i=0;i<50;i++)
											   LcdWrite_Row_Data(2,0,"  Wrong Card  ");
											   
											   return;
											  
											 }
										if (data1[15]==data_buff[15]  ) 
										     {
											   beep1();
											   relay_ON();
											   for(i=0;i<25;i++)
											   LcdWrite_Row_Data(2,0," 001   ");
											   
											   delay1ms(1000);
											   flag=0;
											 }
										if ( data2[15]==data_buff[15] ) 
										     {	
											   beep1();
											   relay_ON();
											   for(i=0;i<25;i++)
											   LcdWrite_Row_Data(2,0," 002   ");
											   
											   delay1ms(1000);
											   flag=0;
											 }
											if(status != MI_OK)
												continue;
											delay1ms(500);
										}	
									}
								}	flag =1;
                                   break;
                         case '2':LcdWriteCom(0x01);
						          
						          LcdWrite_Row_Data(1,0," 2.Add New Cad");
                                  if (PcdRequest(0x52, Temp) == MI_OK)	// Ѱ�����ɹ���Temp����Ϊ������
									{  
										if (PcdAnticoll(UID) == MI_OK)		// ����ͻ��UID��������Ϊ�����к�
										{
											
											status = PcdSelect(UID);		//ѡ����Ƭ
											if(status != MI_OK)
												continue;	   
												 
											status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, UID);	//��֤��Ƭ����
											if(status != MI_OK)
												continue;   
												  
											status = PcdWrite(1, data2);	// д�飬�൱�ڳ�ʼ��Ǯ�����
											if(status != MI_OK)
												continue;		   
											delay1ms(500);
										}
									 }
                      
                                  break;
                         default: break;}
                        }
                       }
                       break;
             case '4': LcdWriteCom(0x01);
                       for(i = 0;i<6;i++)
                       {
                       Test_BUF[i]=At24c02Read(i+1); 
                       }
                       LcdWrite_Row_Data(1,0,Test_BUF);
                       LcdWrite_Row_Data(2,0,KEY_BUF);break;
             case 'D': LcdWriteCom(0x01); 
                       LcdWrite_Row_Data(1,0,"3.IC Input     ");
                       //LcdWrite_Row_Data(2,0,"4.Face ID      ");
					   beep1();
                       break;
             case 'U': LcdWriteCom(0x01);  
                       LcdWrite_Row_Data(1,0,"1.PassWord     ");
                       LcdWrite_Row_Data(2,0,"2.Modify PW    ");
					   beep1();
                       break;
             case 'A': beep1();
                       LcdWriteCom(0x01);
					   while(1)
					   {
					   KeyState=0;
                       KeyDown();
                        if(KeyState)
                        {
                           KeyState=0; 
						   if(dat[KeyValue]!='A')	break;
						}
					   Ds1302ReadTime();
					   LcdDisplay();    
					   } 
                       break;
             default: beep1();break;
            }
        }
    }
}

void  beep1()    
{         
    unsigned int i,j;
    for(i=0;i<600;i++)
    {
      for(j=80;j>0;j--);      
      beep=~beep;
    }
}

void show()
{
    LcdWrite_Row_Data(1,0,dis0);
}




void Delay10ms()  
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}

void KeyDown(void)
{
    char a;
    GPIO_KEY=0x0f;
    if(GPIO_KEY!=0x0f)
    {
        Delay10ms();
        if(GPIO_KEY!=0x0f)
        {
            KeyState=1;

            GPIO_KEY=0X0F;
//          Delay10ms();
            switch(GPIO_KEY)
            {
                case(0X07): KeyValue=1;break;
                case(0X0b): KeyValue=2;break;
                case(0X0d): KeyValue=3;break;
                case(0X0e): KeyValue=13;break;
//          
            }

            GPIO_KEY=0XF0;
            Delay10ms();
            switch(GPIO_KEY)
            {
                case(0X70): KeyValue=KeyValue;break;
                case(0Xb0): KeyValue=KeyValue+3;break;
                case(0Xd0): KeyValue=KeyValue+6;break;
                case(0Xe0): KeyValue=KeyValue+9;break;
//           
            }
            while((a<50)&&(GPIO_KEY!=0xf0)) 
            {
                Delay10ms();
                a++;
            }
            a=0;
        }
    }
}
void UartSendBytes(u8 *c)//�Ӵ��ڷ���16�ֽ�����
{
	u8 i = 0;
	EA = 0;
	for (i=0; i<16; i++)
	{
		SBUF = *c + '0';
		while(!TI);
		TI = 0;
	   	c++;
	}
	SBUF = '\n';
	while (!TI);
		TI = 0;
	EA = 1;
}


void UartSendByte(u8 c)	//�Ӵ��ڷ���1�ֽ�����
{
	u8 i = 0;
	EA = 0;

	SBUF = c;
	while (!TI);
	TI = 0;

	EA = 1;
}

void SysInit(void)
{
    TMOD = 0x20;		// T1����Ϊ8λ�Զ���װ�ض�ʱ��			
    SCON = 0x40;		// ���ڹ�����ģʽ1��8λUART�����ʿɱ䣬�ҽ�ֹ����
    TH1 = 0xE6;			// ��Ƭ��С����V1.3�����2400�������Ҳ�����
    TL1 = TH1;			// �ӱ�ʱ�Ķ�ʱ������ֵ��
    PCON = 0x80;		// ����Ϊ�����ʼӱ�
	EA = 1;				// �����ж�
	ES = 1;			    // �������ж�
	TR1 = 1;		    // ��ʱ��1��������

	//gBuzzer = 0;
    PcdReset();				//��λRC522
    PcdAntennaOff(); 	 	//������ 		
    PcdAntennaOn();  		//������
	M500PcdConfigISOType('A');
}
void LcdDisplay()
{
	LcdWriteCom(0x80+0X40);
	LcdWriteData('0'+TIME[2]/16);				//ʱ
	LcdWriteData('0'+(TIME[2]&0x0f));				 
	LcdWriteData('-');
	LcdWriteData('0'+TIME[1]/16);				//��
	LcdWriteData('0'+(TIME[1]&0x0f));	
	LcdWriteData('-');
	LcdWriteData('0'+TIME[0]/16);				//��
	LcdWriteData('0'+(TIME[0]&0x0f));

	LcdWriteCom(0x80);
	LcdWriteData('2');
	LcdWriteData('0');
	LcdWriteData('0'+TIME[6]/16);			//��
	LcdWriteData('0'+(TIME[6]&0x0f));
	LcdWriteData('-');
	LcdWriteData('0'+TIME[4]/16);			//��
	LcdWriteData('0'+(TIME[4]&0x0f));
	LcdWriteData('-');
	LcdWriteData('0'+TIME[3]/16);			//��
	LcdWriteData('0'+(TIME[3]&0x0f));
	LcdWriteCom(0x8D);
	LcdWriteData('0'+(TIME[5]&0x07));	//����			 
}
void relay_ON(){
relay = 0;
for(i=1;i<=50;i++)
   Delay10ms();
relay = 1;
} 