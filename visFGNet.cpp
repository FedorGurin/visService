#include "visFGNet.h"

#include <stdio.h>
#include <math.h>
#include "globalNameID.h"
#include "math_func.h"
#include "bits.h"
#include <arpa/inet.h>

VisFGNet::VisFGNet(uint32_t idClass,TSettingVisFG set_):ICalculateElement(idClass)
{
    set     = set_;
    solid   = 0;
  
    //! указываем зависимости
    bindTo(new TEventAddrStat(ID_Solid,   (uintptr_t)&solid,    TEventAddrStat::GET_POINTER,this));
    //init();

    UdpSocket *tempSocket = 0;
    for(int i = 0; i<set.port.size(); i++)
    {
        tempSocket = new UdpSocket;
        if(set.ipVis.empty() == false)
            tempSocket->init(set.port[i], set.ipVis[i]);
        else
            tempSocket->init(set.port[i], set.ipStr[i]);

        udpSockets.push_back(tempSocket);                
    }
        
    //! указыем зависимости
    setFreq(ICalculateElement::Hz50);
    setStart();
}
void VisFGNet::init()
{
    sendPosCamera();
    ICalculateElement::init();
}

bool VisFGNet::sendPosCamera()
{
    memset((void*)&data,0,sizeof(data));

    data.version = htonl(FG_NET_FDM_VERSION);

    data.longitude = htond(solid->lam_geo);
    data.altitude  = htond(solid->c_g.y);
    data.latitude  = htond(solid->fi_geo);
    data.psi = htonf(-solid->psi);
    data.phi = htonf( solid->gamma);

    data.theta = htonf(solid->tan);
    data.alpha = htonf(solid->alfa);
    data.beta  = htonf(solid->beta);

    data.cur_time    = (long int)1234567890;
    data.warp        += htonl(1);
    data.visibility  = 0;

    data.v_north    = htonf( solid->vg.x);
    data.v_down     = htonf(-solid->vg.y);
    data.v_body_w   = htonf( solid->vg.z);

   for(int i = 0; i<udpSockets.size(); i++)
   {
       udpSockets[i]->sendTo((uint8_t*)&data,sizeof(data));
   }       
   
   return true;
}
void VisFGNet::calculate()
{
   sendPosCamera();
}


