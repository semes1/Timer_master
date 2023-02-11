/*******************************************
	*	@file ��  bsp_interface.c
	* @author��  �޳�
	* @data�� 2023.01.15
	* @version��  v1.0
*******************************************/

#include "bsp_interface.h"
#include "bsp_button.h"
#include "ili9486.h"
#include "bsp_exti.h"
#include "bsp_usart.h"
#include "bsp_timer.h"

u16 Page_Start = 0;                    //ҳ��
u16 Data1_Seek = 0;                    //�������ݶ�λֵ
u16 Data2_Seek = 0;                    //��Ȧ���ݶ�λֵ
u8 SelectFlag = 1;                     //ѡ���־
u8 InterfaceFlag = 0;                 //�����־
vu8 StatusFlag = 0;                    //״̬��־

extern uint8_t Connection_count;


/*
	*APIname��MainMenu()
	*brief��  ��������ʾ����
	*param��  ��
	*retval�� ��
	*author�� �޳�
	*data��   2023.1.15
*/
void MainMenu(void)                     
{
	ILI9486_clear_screen(0, 80, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH-40);//����

	vTaskDelay(100);
	
	ILI9486_showstring_Ch(88, 80, (u8*)"��ʱ��", GB2312_32X32);
	ILI9486_showstring_En(184, 80, (u8*)"6.0", ASCII_16X32);
	ILI9486_showstring_Ch(100, 140, (u8*)"�񡢵�Ȧ����", GB2312_24X24);
	ILI9486_showstring_Ch(100, 180, (u8*)"�򡢶�ӻ�����", GB2312_24X24);

	if (SelectFlag == 1)
	{
		ILI9486_showstring_Ch(70, 140, (u8*)"��", GB2312_24X24);
	}
	else if(SelectFlag == 2)
	{
		ILI9486_showstring_Ch(70, 180, (u8*)"��", GB2312_24X24);
	}
	button[Up].ButtonDraw(&button[Up]);                                        
	button[Down].ButtonDraw(&button[Down]);
	button[Determine].ButtonDraw(&button[Determine]);
}


/*
	*APIname��Lap_Test_Menu()
	*brief��  ��Ȧ���Խ�����ʾ����
	*param��  ��
	*retval�� ��
	*author�� �޳�
	*data��   2023.1.15
*/
void Lap_Test_Menu(void)
{
	ILI9486_clear_screen(0, 80, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH);//����
	ILI9486_showstring_Ch(100, 80, (u8*)"��Ȧ����", GB2312_32X32);
	ILI9486_showstring_Ch(20, 140, (u8*)"״̬��", GB2312_24X24);
	ILI9486_showstring_Ch(141, 140, (u8*)"����", GB2312_24X24);   	
	ILI9486_draw_rectangle(130, 137, 150, 30, BLUE);

	ILI9486_draw_rectangle(20, 180, 280, 100, BLUE);
	ILI9486_showstring_Ch(30, 200, (u8*)"���β������ݣ�", GB2312_24X24);
	ILI9486_showstring_Ch(30, 240, (u8*)"�ϴβ������ݣ�", GB2312_24X24);

	button[View].ButtonDraw(&button[View]);
	button[Ready].ButtonDraw(&button[Ready]);
	button[Backtrack].ButtonDraw(&button[Backtrack]);
}

/*
	*��������Show_Data()
	*��  �ܣ���ʾ��ǰ��������
	*��  ���� zdata����������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2020.11.07
*/
void Show_Data(u16 zdata, u16 pos_y)
{
	u16 temp = zdata;
	u8 TestData[20] = { 0 };
	u8 num = 0;

	while (temp)
	{
		TestData[num++] = temp % 10 + 0x30;
		temp /= 10;
	}

	u16 pos_x = 210;
	
	if(zdata < 100)
	{
		ILI9486_showchar_En(pos_x, pos_y, '0', ASCII_12X24);
		pos_x += 12;
	}

	while (num)
	{
		num--;
		if (num == 1)
		{
			ILI9486_showchar_En(pos_x, pos_y, '.', ASCII_12X24);
			pos_x += 12;
		}
		ILI9486_showchar_En(pos_x, pos_y, TestData[num], ASCII_12X24);
		pos_x += 12;
	}
}

/*
	*APIname��Connection_Count_Show()
	*brief��  �ӻ�����������ʾ����
	*param��  ��
	*retval�� ��
	*author�� �޳�
	*data��   2023.1.15
*/
void Connection_Count_Show(void)                                   
{
	ILI9486_clear_screen(150, 10, 100, 44);
	ILI9486_draw_rectangle(150, 10, 100, 44, BLUE);
	ILI9486_showstring_Ch(20, 20, (u8*)"����ͨѶ��", GB2312_24X24);
	if(Connection_count != 0)
	{
		ILI9486_showstring_En(160, 20, (u8*)Connection_count, ASCII_12X24);
	}
	else
	{
		ILI9486_showstring_Ch(160, 20, (u8*)"�޴ӻ�����", GB2312_24X24);
	}
}

/*
	*APIname��Connection_Status_Show()
	*brief��  �ӻ���������ʾ����
	*param��  ��
	*retval�� ��
	*author�� �޳�
	*data��   2023.1.15
*/

//
void Slaver_Check_Interface(void)
{
	Connection_count = 0;
	ILI9486_clear_screen(0, 0, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH);
	ILI9486_showstring_Ch(20, 220, (u8*)"����ͨѶ�����", GB2312_24X24);
	ILI9486_showstring_En(190, 220, (u8*)".......", ASCII_12X24);
	
	button[Determine].ButtonDraw(&button[Determine]);
	button[Backtrack].ButtonDraw(&button[Backtrack]);
}


/*
	*APIname��Multi_slave_function_interface()
	*brief��  ��ӻ����ܽ�����ʾ����
	*param��  ��
	*retval�� ��
	*author�� �޳�
	*data��   2023.1.15
*/

void Multi_slave_function_interface(void)
{
	uint8_t i = 0;
	char str[20] = {0};
	Exti_Close();
	Timer3_Off();
	ILI9486_clear_screen(0, 80, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH);    //����	
	
	ILI9486_draw_rectangle(130, 137, 150, 30, BLUE);
	ILI9486_showstring_Ch(100, 80, (u8*)"��ӻ�����", GB2312_32X32);
	ILI9486_showstring_Ch(20, 140, (u8*)"״̬��", GB2312_24X24);
	ILI9486_showstring_Ch(141, 140, (u8*)"����", GB2312_24X24);                         //��Ҫ���Թ۲�����
	while(Connection_count--)
	{
		ILI9486_draw_rectangle(20, 180, 280, Connection_count*50, BLUE);
		sprintf(str,"%s��%d","���β�������",i+1);
		ILI9486_showstring_Ch(30, (200+i*40), (u8*)str, GB2312_24X24);
		i++;
	}
	button[View].ButtonDraw(&button[View]);
	button[Ready].ButtonDraw(&button[Ready]);
	button[Backtrack].ButtonDraw(&button[Backtrack]);
}

/*
	*��������Show_LapRecords()
	*��  �ܣ���ʾ��Ȧ���Լ�¼
	*��  ���� zdata: ���ٲ������ݻ�����
	*����ֵ�� ��
	*��  �ߣ� ����,�޳�
	*��  �ڣ� 2021.09.02 �޸ģ�2023.1.15
*/
void Show_LapRecords(u16* zdata, u16 DataCount)
{
	u16 temp;
	u8 num = 0;
	u8 bit = 0;
	u16 pos_y = 120; 
	u16 pos_x = 200;
	u8 TestData[6] = { 0 };
	char StrBuf[5] = { 0 };
	
	ILI9486_clear_screen(0, 80, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH);//����	
	
	/* �������ݶ�λ��0 */
	Data2_Seek = 0;
	
	/* ҳ�׹�0 */
	Page_Start = 0;
	
	ILI9486_showstring_Ch(110, 80, (u8*)"��Ȧ��������", GB2312_24X24);
	
	/* �жϲ��������Ƿ�Ϊ�� */
	if(DataCount == 0)
	{
		ILI9486_draw_rectangle(60,200,240,50,BLUE);
		ILI9486_showstring_Ch(76, 210, (u8*)"��Ȧ��������Ϊ�գ�", GB2312_24X24);
	}
	else
	{
			/* ��ʾ��ǰ8������ */
		while(num < 8 && Data2_Seek < DataCount)
		{
			ILI9486_showchar_Ch(20, pos_y, (u8*)"��", GB2312_24X24);
			sprintf(StrBuf, "%-3d", Data2_Seek+1);
			ILI9486_showstring_En(44, pos_y, (u8*)StrBuf, ASCII_12X24);
			ILI9486_showstring_Ch(80, pos_y, (u8*)"��������ݣ�", GB2312_24X24);

			temp = zdata[Data2_Seek];
			
			bit = 0;
			while (temp)
			{
				TestData[bit++] = temp % 10 + 0x30;
				temp /= 10;
			}

			pos_x = 210;
			
			if(zdata[Data2_Seek] < 100)
			{
				ILI9486_showchar_En(pos_x, pos_y, '0', ASCII_12X24);
				pos_x += 12;
			}

			/* ��ʾ���� */
			while (bit)
			{
				bit--;
				if (bit == 1)
				{
					ILI9486_showchar_En(pos_x, pos_y, '.', ASCII_12X24);
					pos_x += 12;
				}
				ILI9486_showchar_En(pos_x, pos_y, TestData[bit], ASCII_12X24);
				pos_x += 12;
			}
			Data2_Seek++;
			num++;
			pos_y += 30;
		}
	}
	button[Up].ButtonDraw(&button[Up]);                  
	button[Down].ButtonDraw(&button[Down]);
	button[Empty].ButtonDraw(&button[Empty]);
	button[Backtrack].ButtonDraw(&button[Backtrack]);
}

/*
	*��������Show_AcceleratedRecords()
	*��  �ܣ���ʾ���ٲ��Լ�¼
	*��  ���� zdata: ���ٲ������ݻ�����
	*����ֵ�� ��
	*��  �ߣ� ����,�޳�
	*��  �ڣ� 2021.09.02 �޸ģ�2023.1.15
*/
void Show_AcceleratedRecords(u16* zdata, u16 DataCount)
{
	ILI9486_clear_screen(0, 80, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH);//����

	u16 temp;
	u8 bit = 0;
	u16 pos_y = 120; 
	u16 pos_x = 200;
	u8 TestData[6] = { 0 };
	char StrBuf[5] = { 0 };
	
	/* �������ݶ�λ��0 */
	Data1_Seek = 0;
	
	/* ҳ�׹�0 */
	Page_Start = 0;
	
	ILI9486_showstring_Ch(110, 80, (u8*)"���ٲ�������", GB2312_24X24);
	
	/* �жϲ��������Ƿ�Ϊ�� */
	if(DataCount == 0)
	{
		ILI9486_draw_rectangle(60,200,240,50,BLUE);
		ILI9486_showstring_Ch(76, 210, (u8*)"���ٲ�������Ϊ�գ�", GB2312_24X24);
	}
	else
	{
			/* ��ʾ��ǰ8������ */
		while(Data1_Seek < 8 && Data1_Seek < DataCount)
		{
			/* ��ʾ��ʾ��Ϣ */
			ILI9486_showchar_Ch(20, pos_y, (u8*)"��", GB2312_24X24);
			sprintf(StrBuf, "%-3d", Data1_Seek+1);
			ILI9486_showstring_En(44, pos_y, (u8*)StrBuf, ASCII_12X24);
			ILI9486_showstring_Ch(80, pos_y, (u8*)"��������ݣ�", GB2312_24X24);

			temp = zdata[Data1_Seek];
			
			bit = 0;
			while (temp)
			{
				TestData[bit++] = temp % 10 + 0x30;
				temp /= 10;
			}

			pos_x = 210;
			
			if(zdata[Data1_Seek] < 100)
			{
				ILI9486_showchar_En(pos_x, pos_y, '0', ASCII_12X24);
				pos_x += 12;
			}
		
			/* ��ʾ���� */
			while (bit)
			{
				bit--;
				if (bit == 1)
				{
					ILI9486_showchar_En(pos_x, pos_y, '.', ASCII_12X24);
					pos_x += 12;
				}
				ILI9486_showchar_En(pos_x, pos_y, TestData[bit], ASCII_12X24);
				pos_x += 12;
			}
			Data1_Seek++;
			pos_y += 30;
		}
	}
	button[Up].ButtonDraw(&button[Up]);
	button[Down].ButtonDraw(&button[Down]);
	button[Empty].ButtonDraw(&button[Empty]);
	button[Backtrack].ButtonDraw(&button[Backtrack]);
}



/*
	*��������Page_Down()
	*��  �ܣ���һҳ
	*��  ���� 
	*����ֵ�� ��
	*��  �ߣ� ����,�޳�
	*��  �ڣ� 2021.09.01  �޸ģ�2023.1.15
*/
void Page_Down(u16* zdata, u16 DataCount)
{
	/* ��������һҳ */
	if(Data1_Seek >= DataCount || Data2_Seek >= DataCount)
	{
		return;
	}
	
	ILI9486_clear_screen(0, 120, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH - 160);//����
	
	u16 temp;
	u8 num = 0;
	u8 bit = 0;
	u16 pos_y = 120; 
	u16 pos_x = 200;
	u8 TestData[6] = { 0 };
	char StrBuf[5] = { 0 };
	
	/* ����Ǽ���������ʾ���� */
	if(5 == InterfaceFlag)
	{
		Page_Start = Data1_Seek;
		
		/* ��ʾ��ǰ8������ */
		while(num < 8 && Data1_Seek < DataCount)
		{
			ILI9486_showchar_Ch(20, pos_y, (u8*)"��", GB2312_24X24);
			sprintf(StrBuf, "%-3d", Data1_Seek+1);
			ILI9486_showstring_En(44, pos_y, (u8*)StrBuf, ASCII_12X24);
			ILI9486_showstring_Ch(80, pos_y, (u8*)"��������ݣ�", GB2312_24X24);

			temp = zdata[Data1_Seek];
			
			bit = 0;
			while (temp)
			{
				TestData[bit++] = temp % 10 + 0x30;
				temp /= 10;
			}

			pos_x = 210;
			
			if(zdata[Data1_Seek] < 100)
			{
				ILI9486_showchar_En(pos_x, pos_y, '0', ASCII_12X24);
				pos_x += 12;
			}

			while (bit)
			{
				bit--;
				if (bit == 1)
				{
					ILI9486_showchar_En(pos_x, pos_y, '.', ASCII_12X24);
					pos_x += 12;
				}
				ILI9486_showchar_En(pos_x, pos_y, TestData[bit], ASCII_12X24);
				pos_x += 12;
			}
			Data1_Seek++;
			num++;
			pos_y += 30;
		}
	}
	/* ����ǵ�Ȧ������ʾ���� */
	else if(4 == InterfaceFlag)
	{
		Page_Start = Data2_Seek;
		
		/* ��ʾ��ǰ8������ */
		while(num < 8 && Data2_Seek < DataCount)
		{
			ILI9486_showchar_Ch(20, pos_y, (u8*)"��", GB2312_24X24);
			sprintf(StrBuf, "%-3d", Data2_Seek+1);
			ILI9486_showstring_En(44, pos_y, (u8*)StrBuf, ASCII_12X24);
			ILI9486_showstring_Ch(80, pos_y, (u8*)"��������ݣ�", GB2312_24X24);

			temp = zdata[Data2_Seek];
			
			bit = 0;
			while (temp)
			{
				TestData[bit++] = temp % 10 + 0x30;
				temp /= 10;
			}

			pos_x = 210;
			
			if(zdata[Data2_Seek] < 100)
			{
				ILI9486_showchar_En(pos_x, pos_y, '0', ASCII_12X24);
				pos_x += 12;
			}

			while (bit)
			{
				bit--;
				if (bit == 1)
				{
					ILI9486_showchar_En(pos_x, pos_y, '.', ASCII_12X24);
					pos_x += 12;
				}
				ILI9486_showchar_En(pos_x, pos_y, TestData[bit], ASCII_12X24);
				pos_x += 12;
			}
			Data2_Seek++;
			num++;
			pos_y += 30;
		}
	}
}

/*
	*��������Page_Up()
	*��  �ܣ���һҳ
	*��  ���� 
	*����ֵ�� ��
	*��  �ߣ� ���壬�޳�
	*��  �ڣ� 2021.09.01 ���޸ģ�2023.1.15
*/
void Page_Up(u16* zdata, u16 DataCount)
{
	u16 temp;
	u8 num = 0;
	u8 bit = 0;
	u16 pos_y = 120;
	u16 pos_x = 200;
	u8 TestData[6] = { 0 };
	char StrBuf[5] = { 0 };	
	/* ����ǵ�һҳֱ�ӷ��� */
	if(Page_Start == 0)
	{
		return;
	}
	ILI9486_clear_screen(0, 120, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH - 160);//����
	/* ����Ǽ���������ʾ���� */
	if(5 == InterfaceFlag)
	{
		if(Page_Start > 8)
		{
			Page_Start -= 9;
		}
		else 
		{
			Page_Start = 0;
		}
		
		Data1_Seek = Page_Start;
		
		/* ��ʾ��ǰ8������ */
		while(num < 8)
		{
			ILI9486_showchar_Ch(20, pos_y, (u8*)"��", GB2312_24X24);
			sprintf(StrBuf, "%-3d", Data1_Seek+1);
			ILI9486_showstring_En(44, pos_y, (u8*)StrBuf, ASCII_12X24);
			ILI9486_showstring_Ch(80, pos_y, (u8*)"��������ݣ�", GB2312_24X24);

			temp = zdata[Data1_Seek];
			
			bit = 0;
			while (temp)
			{
				TestData[bit++] = temp % 10 + 0x30;
				temp /= 10;
			}

			pos_x = 210;
			
			if(zdata[Data1_Seek] < 100)
			{
				ILI9486_showchar_En(pos_x, pos_y, '0', ASCII_12X24);
				pos_x += 12;
			}

			while (bit)
			{
				bit--;
				if (bit == 1)
				{
					ILI9486_showchar_En(pos_x, pos_y, '.', ASCII_12X24);
					pos_x += 12;
				}
				ILI9486_showchar_En(pos_x, pos_y, TestData[bit], ASCII_12X24);
				pos_x += 12;
			}
			Data1_Seek++;
			num++;
			pos_y += 30;
		}
	}
	/* ������ڵ�Ȧ������ʾ���� */
	else if(4 == InterfaceFlag)
	{
		if(Page_Start > 8)
		{
			Page_Start -= 9;
		}
		else 
		{
			Page_Start = 0;
		}
		
		Data2_Seek = Page_Start;
		
		/* ��ʾ��ǰ8������ */
		while(num < 8)
		{
			ILI9486_showchar_Ch(20, pos_y, (u8*)"��", GB2312_24X24);
			sprintf(StrBuf, "%-3d", Data2_Seek+1);
			ILI9486_showstring_En(44, pos_y, (u8*)StrBuf, ASCII_12X24);
			ILI9486_showstring_Ch(80, pos_y, (u8*)"��������ݣ�", GB2312_24X24);

			temp = zdata[Data2_Seek];
			
			bit = 0;
			while (temp)
			{
				TestData[bit++] = temp % 10 + 0x30;
				temp /= 10;
			}

			pos_x = 210;
			
			if(zdata[Data2_Seek] < 100)
			{
				ILI9486_showchar_En(pos_x, pos_y, '0', ASCII_12X24);
				pos_x += 12;
			}

			while (bit)
			{
				bit--;
				if (bit == 1)
				{
					ILI9486_showchar_En(pos_x, pos_y, '.', ASCII_12X24);
					pos_x += 12;
				}
				ILI9486_showchar_En(pos_x, pos_y, TestData[bit], ASCII_12X24);
				pos_x += 12;
			}
			Data2_Seek++;
			num++;
			pos_y += 30;
		}
	}
}

/*
	*��������Empty_Records()
	*��  �ܣ���ղ��Լ�¼
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2020.10.24
*/
void Empty_Records()
{
	InterfaceFlag += 2;
	ILI9486_clear_screen(0, 80, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH - 40);//����
	ILI9486_draw_rectangle(60, 200, 200, 80, BLUE);
	if(InterfaceFlag == 6)
	{
		ILI9486_showstring_Ch(88, 210, (u8*)"ȷ����ռ���", GB2312_24X24);
		ILI9486_showstring_Ch(98, 250, (u8*)"�������ݣ�", GB2312_24X24);
	}
	else if(InterfaceFlag == 7)
	{
		ILI9486_showstring_Ch(88, 210, (u8*)"ȷ����յ�Ȧ", GB2312_24X24);
		ILI9486_showstring_Ch(98, 250, (u8*)"�������ݣ�", GB2312_24X24);
	}
	button[Determine].start_x = button[Up].start_x;
	button[Determine].start_y = button[Up].start_y;
	button[Determine].end_x = button[Up].end_x;
	button[Determine].end_y = button[Up].end_y;
	
	button[Determine].ButtonDraw(&button[Determine]);
	button[Backtrack].ButtonDraw(&button[Backtrack]);
	
	button[Determine].start_x = button[Backtrack].start_x;
	button[Determine].start_y = button[Backtrack].start_y;
	button[Determine].end_x = button[Backtrack].end_x;
	button[Determine].end_y = button[Backtrack].end_y;
}


/*
	*��������Empty_Insure()
	*��  �ܣ����ȷ��
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2020.10.24
*/
void  Empty_Insure(void)
{
	ILI9486_clear_screen(0, 80, ILI9486_SCREEN_LESSWIDTH, ILI9486_SCREEN_MOREWIDTH);//����
	ILI9486_draw_rectangle(100, 200, 120, 80, BLUE);
	ILI9486_showstring_Ch(112, 210, (u8*)"����ɾ��", GB2312_24X24);
	ILI9486_showstring_Ch(122, 250, (u8*)"�ɹ���", GB2312_24X24);

	button[Backtrack].ButtonDraw(&button[Backtrack]);
}