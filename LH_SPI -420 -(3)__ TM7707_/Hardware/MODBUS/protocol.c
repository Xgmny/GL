#include "protocol.h"
#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������
//MODBUS Э�����������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2017-2027
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


m_protocol_dev_typedef m_ctrl_dev;	//����modbus������


//����һ֡���ݣ���������洢��fx����
//ע�⣺���������õ�malloc��fx����ָ�������ڴ棬��������fx��һ��Ҫ�ͷ��ڴ棡��
//����������ڴ�й¶������
//fx��ָ֡�� 
//����ֵ�����������0��OK��������������롣
m_result mb_unpack_frame(m_frame_typedef *fx)
{
 	u16 rxchkval=0;   	 		//���յ���У��ֵ 
	u16 calchkval=0;			//����õ���У��ֵ	
 	u8 datalen=0; 				//��Ч���ݳ��� 
	fx->datalen=0; 				//���ݳ�������
	if(m_ctrl_dev.rxlen>M_MAX_FRAME_LENGTH||m_ctrl_dev.rxlen<M_MIN_FRAME_LENGTH)
	{
		m_ctrl_dev.rxlen=0;			//���rxlen
		m_ctrl_dev.frameok=0;		//���framok��ǣ��Ա��´ο����������� 
		return MR_FRAME_FORMAT_ERR;//֡��ʽ����
	}
	datalen=m_ctrl_dev.rxbuf[3];
	switch(m_ctrl_dev.checkmode)
	{
		case M_FRAME_CHECK_SUM:							//У���
			calchkval=mc_check_sum(m_ctrl_dev.rxbuf,datalen+4);
			rxchkval=m_ctrl_dev.rxbuf[datalen+4];
			break;
		case M_FRAME_CHECK_XOR:							//���У��
			calchkval=mc_check_xor(m_ctrl_dev.rxbuf,datalen+4);
			rxchkval=m_ctrl_dev.rxbuf[datalen+4];
			break;
		case M_FRAME_CHECK_CRC8:						//CRC8У��
			calchkval=mc_check_crc8(m_ctrl_dev.rxbuf,datalen+4);
			rxchkval=m_ctrl_dev.rxbuf[datalen+4];
			break;
		case M_FRAME_CHECK_CRC16:						//CRC16У��
			calchkval=mc_check_crc16(m_ctrl_dev.rxbuf,datalen+4);
			rxchkval=((u16)m_ctrl_dev.rxbuf[datalen+4]<<8)+m_ctrl_dev.rxbuf[datalen+5];
			break;
	} 	
	m_ctrl_dev.rxlen=0;			//���rxlen
	m_ctrl_dev.frameok=0;		//���framok��ǣ��Ա��´ο����������� 
	if(calchkval==rxchkval)		//У������
	{
		fx->address=m_ctrl_dev.rxbuf[0];
		fx->function=m_ctrl_dev.rxbuf[1];
		fx->count=m_ctrl_dev.rxbuf[2];
		fx->datalen=m_ctrl_dev.rxbuf[3];
		if(fx->datalen)
		{
			fx->data=mymalloc(SRAMIN,fx->datalen);		//�����ڴ�
			for(datalen=0;datalen<fx->datalen;datalen++)
			{
				fx->data[datalen]=m_ctrl_dev.rxbuf[4+datalen];		//��������
			}
		}
		fx->chkval=rxchkval;	//��¼У��ֵ		
	}else return MR_FRAME_CHECK_ERR;
	return MR_OK;	
}


//���һ֡���ݣ������� 
//fx��ָ����Ҫ�����֡  
void mb_packsend_frame(m_frame_typedef *fx)
{  
	u16 i;
	u16 calchkval=0;			//����õ���У��ֵ	
	u16 framelen=0;				//������֡����
	u8 *sendbuf;				//���ͻ�����
	
	if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16)framelen=6+fx->datalen;
	else framelen=5+fx->datalen;
	sendbuf=mymalloc(SRAMIN,framelen);	//�����ڴ�
	sendbuf[0]=fx->address;
	sendbuf[1]=fx->function;
	sendbuf[2]=fx->count;
	sendbuf[3]=fx->datalen; 
	for(i=0;i<fx->datalen;i++)
	{
		sendbuf[4+i]=fx->data[i];
	}	
	switch(m_ctrl_dev.checkmode)
	{
		case M_FRAME_CHECK_SUM:							//У���
			calchkval=mc_check_sum(sendbuf,fx->datalen+4); 
			break;
		case M_FRAME_CHECK_XOR:							//���У��
			calchkval=mc_check_xor(sendbuf,fx->datalen+4); 
			break;
		case M_FRAME_CHECK_CRC8:						//CRC8У��
			calchkval=mc_check_crc8(sendbuf,fx->datalen+4); 
			break;
		case M_FRAME_CHECK_CRC16:						//CRC16У��
			calchkval=mc_check_crc16(sendbuf,fx->datalen+4); 
			break;
	} 
	
	if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16)		//�����CRC16,����2���ֽڵ�CRC
	{
		sendbuf[4+fx->datalen]=(calchkval>>8)&0XFF; 	//���ֽ���ǰ
		sendbuf[5+fx->datalen]=calchkval&0XFF;			//���ֽ��ں�
	}else sendbuf[4+fx->datalen]=calchkval&0XFF;
	mp_send_data(sendbuf,framelen);	//������һ֡����
	myfree(SRAMIN,sendbuf);			//�ͷ��ڴ�
}


//���һ֡���ݣ������� 
//fx��ָ����Ҫ�����֡  
void mb_mod(m_modbus_typedef  *fx)
{  
	u16 i;
	u16 calchkval=0;			//����õ���У��ֵ	
	u16 framelen=0;				//������֡����
	u8 sendbuf[8];				//���ͻ�����
	



	sendbuf[0]=fx->m_1;
	sendbuf[1]=fx->m_2;
	sendbuf[2]=fx->m_3;
	sendbuf[3]=fx->m_4;
	sendbuf[4]=fx->m_5;
	sendbuf[5]=fx->m_6;
	
	calchkval=mc_check_crc16(sendbuf,6); 

//	sendbuf[6]=(calchkval>>8)&0XFF; 	//���ֽ���ǰ
//	sendbuf[7]=calchkval&0XFF;			//���ֽ��ں�

	mp_send_data(sendbuf,6);	//������һ֡����

}

//��ʼ��modbus
//checkmode��У��ģʽ��0,У���;1,���;2,CRC8;3,CRC16
//����ֵ��0,�ɹ�;����,�������
m_result mb_init(u8 checkmode)
{
	m_ctrl_dev.rxbuf=mymalloc(SRAMIN,M_MAX_FRAME_LENGTH);	//��������֡���ջ���
	m_ctrl_dev.rxlen=0;
	m_ctrl_dev.frameok=0;
	m_ctrl_dev.checkmode=checkmode;
	if(m_ctrl_dev.rxbuf)return MR_OK;
	else return MR_MEMORY_ERR;
}

//����MODBUS,�ͷ��ڴ�
void mb_destroy(void)
{
	myfree(SRAMIN,m_ctrl_dev.rxbuf);	//��������֡���ջ���
	m_ctrl_dev.rxlen=0;
	m_ctrl_dev.frameok=0; 
}



