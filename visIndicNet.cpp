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

    UdpSocket *tempSocket = 0;
    for(int i = 0; i < set.port.size(); i++)
    {
        tempSocket = new UdpSocket;
        if(set.ipVis.empty() == false)
            tempSocket->init(set.port[i], set.ipVis[i]);
        else
            tempSocket->init(set.port[i], set.ipStr[i]);

        udpSockets.push_back(tempSocket);
    }   
    //! указыем частоту
    setFreq(ICalculateElement::Hz50);
    setStart();
}
void VisIndicNet::init()
{
    ICalculateElement::init();
}
bool VisIndicNet::sendMfi_2()
{
    DataToStream out(buf1,sizeof (buf1));

    out<<(uint32_t)htonl(1);

    DataToStream outPacket(buf2,sizeof(buf2));
    outPacket<<(uint32_t)htonl(2);
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

    for(size_t i = 0; i<udpSockets.size(); i++)
    {
           udpSockets[i]->sendTo(out.buf,out.index);
    }

    return true;



}
bool VisIndicNet::sendMfi_1()
{
//    if(kin == 0)
//        return false;
//    //! обнуление буфера
//    memset((void*)buf,0,sizeof(buf));

//    camera.latitude     = (kin->fi_geo  * radToGrad);
//    camera.longitude    = (kin->lam_geo * radToGrad);
//    camera.altitude     = kin->c_g.y;
//    camera.head         =  psiToKurs(kin->psi_cam) * radToGrad;
//    camera.pitch        = (kin->tan_cam   * radToGrad);
//    camera.roll         = (kin->gamma_cam * radToGrad);
//    camera.vx           = (kin->vg.z);
//    camera.vy           = (kin->vg.x);
//    camera.vz           = (kin->vg.y);
    
//    sizeBuf = 0;
   
//    pos.latitude = camera.latitude ;
//    pos.longitude= camera.longitude;
//    pos.altitude =  camera.altitude;
//    pos.head = camera.head;
//    pos.pitch = camera.pitch;
//    pos.roll = camera.roll;
//    pos.vx = camera.vx;
//    pos.vy = camera.vy;
//    pos.vz = camera.vz;
    
//    rot.b0.w0.Rpm_screw = 900;
//    if(rpm100 != 0)
//    	rot.b0.w0.Rpm_screw *= (*rpm100)/100.0;
//    rot.b0.Height = kin->hRelief;
//    rot.b0.w0.Light_on = 0;
//    rot.b0.w0.Dustiness = 1;
//    int32_t count = ntohl(1);
//   uint32_t *header = (uint32_t *)(&buf[0]);
//   *header = ntohl((uint32_t)_GIS_MAP3D_SET_CAMERA_);
//   memcpy((void*)&(buf[sizeof(uint32_t)]),(void*)&camera,sizeof(camera));
//   memcpy((void*)&(buf[sizeof(uint32_t)+ sizeof(camera)]),(void*)&count,sizeof(int32_t));
//   memcpy((void*)&(buf[sizeof(uint32_t)+ sizeof(camera) + sizeof(int32_t)]),(void*)&head,sizeof(head));
//   memcpy((void*)&(buf[sizeof(uint32_t)+ sizeof(camera) + sizeof(int32_t) + sizeof(head)]),(void*)&pos,sizeof(pos));
     
//   sizeBuf = sizeof(uint32_t) + sizeof(camera)+ sizeof(int32_t) + sizeof(head) + sizeof(pos);
  
//   for(int i = 0; i<udpSockets.size(); i++)
//   {
//       udpSockets[i]->sendTo(buf,sizeBuf);
//   }
   
//   *header = ntohl((uint32_t)_GIS_MAP3D_SET_OBJECT_ANIMATION_);
//   count = htonl((int32_t)sizeof(rot));
//   memcpy((void*)&(buf[sizeof(uint32_t)]),(void*)&head,sizeof(head));
//   memcpy((void*)&(buf[sizeof(uint32_t) + sizeof(head)]),(void*)&count,sizeof(int32_t));
//   memcpy((void*)&(buf[sizeof(uint32_t) + sizeof(head) + sizeof(int32_t)]),(void*)&rot,sizeof(rot));

//   sizeBuf = sizeof(uint32_t) + sizeof(head)+ sizeof(int32_t) + sizeof(rot);

//   for(int i = 0; i<udpSockets.size(); i++)
//   {
//       udpSockets[i]->sendTo(buf,sizeBuf);
//   }
   
   return true;
}
void VisIndicNet::calculate()
{
   sendMfi_1();
   sendMfi_2();
}


