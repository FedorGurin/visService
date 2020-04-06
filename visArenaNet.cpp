#include "visArenaNet.h"

#include <stdio.h>
#include <math.h>
#include "globalNameID.h"
#include "math_func.h"
#include "bits.h"

VisArenaNet::VisArenaNet(uint32_t idClass,TSettingVisArena set_):ICalculateElement(idClass)
{
    set = set_;
  
    kin = 0;
    rpm100 = 0;
    //! указываем зависимости
    bindTo(new TEventAddrStat(ID_I_Kinematic,   (uintptr_t)&kin,    TEventAddrStat::GET_POINTER,this));
    bindTo(new TEventAddrStat(ID_RPM_100,   (uintptr_t)&rpm100,    TEventAddrStat::GET_POINTER,this));
    double             *rpm100;
    //init();

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
    head.timeout = 100;
    head.name    = 0xBEEF;
    head.type    = 0xE2000001;
    head.view.code = 14015;
    head.view.value= 2;

    //! указыем частоту
    setFreq(ICalculateElement::Hz50);
    setStart();
}
void VisArenaNet::init()
{
    //sendCloud();
    sendPosCamera();
    ICalculateElement::init();
}
bool VisArenaNet::sendCloud()
{
    if(kin == 0)
        return false;
    //! обнуление буфера
        memset((void*)buf,0,sizeof(buf));

                  
        cloud.latitude     = (kin->fi_geo  * radToGrad);
        cloud.longitude    = (kin->lam_geo * radToGrad);
        cloud.altitude     = kin->c_g.y;
        cloud.depth        = 100.0;
        cloud.size        = 0.1;
        cloud.signs.cloudType    = 0;
        cloud.signs.cloudDensity = 100;
       
        
        sizeBuf = 0;
       
       uint32_t *header = (uint32_t *)(&buf[0]);
       *header = ntohl((uint32_t)_GIS_MAP3D_SET_CLOUD_);
       memcpy((void*)&(buf[sizeof(uint32_t)]),(void*)&cloud,sizeof(cloud));
       
       sizeBuf = 1*sizeof(uint32_t) + sizeof(cloud);

       for(int i = 0; i<udpSockets.size(); i++)
       {
           udpSockets[i]->sendTo(buf,sizeBuf);        
       }       
       
       return true;
}
bool VisArenaNet::sendPosCamera()
{
    if(kin == 0)
        return false;
    //! обнуление буфера
    memset((void*)buf,0,sizeof(buf));

    camera.latitude     = (kin->fi_geo  * radToGrad);
    camera.longitude    = (kin->lam_geo * radToGrad);
    camera.altitude     = kin->c_g.y;
    camera.head         =  psiToKurs(kin->psi_cam) * radToGrad;
    camera.pitch        = (kin->tan_cam   * radToGrad);
    camera.roll         = (kin->gamma_cam * radToGrad);
    camera.vx           = (kin->vg.z);
    camera.vy           = (kin->vg.x);
    camera.vz           = (kin->vg.y);
    
    sizeBuf = 0;
   
    pos.latitude = camera.latitude ;
    pos.longitude= camera.longitude;
    pos.altitude =  camera.altitude;
    pos.head = camera.head;        
    pos.pitch = camera.pitch;     
    pos.roll = camera.roll;      
    pos.vx = camera.vx;         
    pos.vy = camera.vy;         
    pos.vz = camera.vz;         
    
    rot.b0.w0.Rpm_screw = 900;
    if(rpm100 != 0)
        rot.b0.w0.Rpm_screw *= (*rpm100)/100.0;
    rot.b0.Height = kin->hRelief;
    rot.b0.w0.Light_on = 0;  
    rot.b0.w0.Dustiness = 1;
    int32_t count = ntohl(1);
   uint32_t *header = (uint32_t *)(&buf[0]);
   *header = ntohl((uint32_t)_GIS_MAP3D_SET_CAMERA_);
   memcpy((void*)&(buf[sizeof(uint32_t)]),(void*)&camera,sizeof(camera));
   memcpy((void*)&(buf[sizeof(uint32_t)+ sizeof(camera)]),(void*)&count,sizeof(int32_t));
   memcpy((void*)&(buf[sizeof(uint32_t)+ sizeof(camera) + sizeof(int32_t)]),(void*)&head,sizeof(head));
   memcpy((void*)&(buf[sizeof(uint32_t)+ sizeof(camera) + sizeof(int32_t) + sizeof(head)]),(void*)&pos,sizeof(pos));
     
   sizeBuf = sizeof(uint32_t) + sizeof(camera)+ sizeof(int32_t) + sizeof(head) + sizeof(pos);
  
   for(int i = 0; i<udpSockets.size(); i++)
   {
       udpSockets[i]->sendTo(buf,sizeBuf);        
   }
   
   *header = ntohl((uint32_t)_GIS_MAP3D_SET_OBJECT_ANIMATION_);
   count = htonl((int32_t)sizeof(rot));
   memcpy((void*)&(buf[sizeof(uint32_t)]),(void*)&head,sizeof(head));
   memcpy((void*)&(buf[sizeof(uint32_t) + sizeof(head)]),(void*)&count,sizeof(int32_t));
   memcpy((void*)&(buf[sizeof(uint32_t) + sizeof(head) + sizeof(int32_t)]),(void*)&rot,sizeof(rot));      

   sizeBuf = sizeof(uint32_t) + sizeof(head)+ sizeof(int32_t) + sizeof(rot);

   for(int i = 0; i<udpSockets.size(); i++)
   {
       udpSockets[i]->sendTo(buf,sizeBuf);        
   }       
   
   return true;
}
void VisArenaNet::calculate()
{
   sendPosCamera();
}


