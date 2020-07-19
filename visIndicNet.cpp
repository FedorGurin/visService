#include "visIndicNet.h"

#include <stdio.h>
#include <math.h>
#include "globalNameID.h"
#include "math_func.h"
#include "bits.h"


class ByteArray
{
public:
	ByteArray(uint8_t *ptr, size_t size_)
	{
		buf = ptr;
		size= size_;
	}

	uint8_t *buf;
	size_t size;
};
//! данные в буфер
class DataToStream
{
public:
    DataToStream(uint8_t *ptr, uint32_t size)
    {
    	index = 0;
        buf     = ptr;
        //bufBegin= ptr;
        bufSize = size;
    }

    DataToStream& operator<<(uint16_t value)
    {
        memcpy((void*)&(buf[index]),(void*)&value,sizeof(value));
        index += sizeof (value);
        return *this;
    }
    DataToStream& operator<<(float value)
    {
        memcpy((void*)&(buf[index]),(void*)&value,sizeof(value));
        index += sizeof (value);
        return *this;
    }
    DataToStream& operator<<(uint32_t value)
    {
        memcpy((void*)&(buf[index]),(void*)&value,sizeof(value));
        index += sizeof (value);
        return *this;
    }
    DataToStream& operator<<(uint8_t value)
    {
        memcpy((void*)&(buf[index]),(void*)&value,sizeof(value));
        index += sizeof (value);
        return *this;
    }
   DataToStream& operator<<(ByteArray array)
    {
        memcpy((void*)&(buf[index]),(void*)array.buf,array.size);
        index += array.size;
        return *this;
    }
    //! текущая позиция в потоке
    uint32_t  index;
    //! буффер с данными
    uint8_t *buf;
    //uint8_t *bufBegin;
    uint32_t bufSize;
};


VisIndicNet::VisIndicNet(uint32_t idClass,TSettingVisIndic set_):ICalculateElement(idClass)
{
    set = set_;


    //! указываем зависимости
    bindTo(new TEventAddrStat(ID_I_Kinematic,   (uintptr_t)&kin,    TEventAddrStat::GET_POINTER,this));
    bindTo(new TEventAddrStat(ID_MFI141_L_PIL,   (uintptr_t)&mfi_1,    TEventAddrStat::GET_POINTER,this));
    bindTo(new TEventAddrStat(ID_MFI141_R_PIL,   (uintptr_t)&mfi_2,    TEventAddrStat::GET_POINTER,this));

    udpSocketMFI = new UdpSocket;
    udpSocketMFI->init(set.portMFI1, set.ipVisSKI);

    udpSocketIM = new UdpSocket;
    udpSocketIM->init(set.portIM, set.ipVisSKI);

    //! указыем частоту
    setFreq(ICalculateElement::Hz50);
    setStart();
}
void VisIndicNet::init()
{
    ICalculateElement::init();
}
bool VisIndicNet::sendMfi_1()
{
    DataToStream out(buf1,sizeof (buf1));

    out<<(uint32_t)htonl(1);

    DataToStream outPacket(buf2,sizeof(buf2));
    outPacket<<(uint32_t)htonl(1);
    outPacket<<(uint16_t)htons(70);
    outPacket<<(uint32_t)htonl(0xFF454545);
    outPacket<<(uint16_t)htons(1);
    outPacket<<(uint16_t) htons(227);
    outPacket<<(uint16_t) htons(sizeof(SMfdPilResearchOutput));
    ByteArray array((uint8_t*) mfi_1,sizeof(SMfdPilResearchOutput));
    outPacket<<array;


    out<<(uint32_t) htonl(outPacket.index);
    ByteArray arrayPacket((uint8_t*) outPacket.buf,outPacket.index);
    out<<arrayPacket;

    udpSocketMFI->sendTo(out.buf,out.index);
    //for(size_t i = 0; i<udpSockets.size(); i++)
    //{
          // udpSockets[i]->sendTo(out.buf,out.index);
    //}

    return true;

}
bool VisIndicNet::sendIM_1()
{
	 DataToStream out(buf1,sizeof (buf1));

	 out<<(uint32_t)htonl(1);

	 DataToStream outPacket(buf2,sizeof(buf2));
	 outPacket<<(uint32_t)htonl(2);
	 outPacket<<(uint16_t)htons(70);
	 outPacket<<(uint32_t)htonl(0xFF454545);
	 outPacket<<(uint16_t)htons(1);
	 outPacket<<(uint16_t) htons(228);
	 outPacket<<(uint16_t) htons(sizeof(SMfdPilResearchOutput));
	 ByteArray array((uint8_t*) mfi_1,sizeof(SMfdPilResearchOutput));
	 outPacket<<array;


	 out<<(uint32_t) htonl(outPacket.index);
	 ByteArray arrayPacket((uint8_t*) outPacket.buf,outPacket.index);
	 out<<arrayPacket;

	 udpSocketIM->sendTo(out.buf,out.index);


   return true;
}
void VisIndicNet::calculate()
{
   sendMfi_1();
   sendIM_1();
}


