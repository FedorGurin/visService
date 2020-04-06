#include "reliefArenaNet.h"

#include <stdio.h>
#include <math.h>
#include "globalNameID.h"
#include "math_func.h"
#include "bits.h"
#include <iomanip>

ReliefArenaNet::ReliefArenaNet(uint32_t idClass, TSettingReliefArena set_):ICalculateElement(idClass)
{
    //! задать настройки
    set = set_;    
    udpSocket    = 0;
    udpSocketRec = 0;
    //! указываем зависимости
    bindTo(new TEventAddrStat(ID_I_Kinematic,   (uintptr_t)&kin,    TEventAddrStat::GET_POINTER,this));
    //! указыем зависимости
    setFreq(ICalculateElement::Hz6_25);
    setStart();
}
void ReliefArenaNet::init()
{
    if(udpSocket!=0 && udpSocketRec!=0)
    {
        ICalculateElement::init();
        return;
    }
    if(udpSocket == 0)
    {
        udpSocket = new UdpSocket;
        udpSocket->init(set.portSend, set.ipVis);
    }
    if(udpSocketRec == 0)
    {
        udpSocketRec = new UdpSocket;
        udpSocketRec->init(set.portRecive,set.ipOwn);
    }
    
    if(udpSocketRec->bindTo() == false)
        std::cout<<"ReliefArenaNet: Can`t bind to socket"<<std::endl;

    ipOwn = set.ipOwn;

    //! проверяем подключение к СВ каждые 1000 сек
    udpSocketRec->setTimeout(set.dtCheckConnectMs);
    waitingReliefAns();   
        
    //! теперь сокет не блокируемый    
    if(udpSocketRec->setBlock(false) == false)
        std::cout<<"ReliefArenaNet: Сan`t set blocking socket"<<std::endl;

    ICalculateElement::init();
}
void ReliefArenaNet::calculate()
{
    //! получение данных
    reciveAnswer();    
    //! отправка данных
    sendRequest();
}
void  ReliefArenaNet::waitingReliefAns()
{
    float dt = set.dtCheckConnectMs/1000.0;
    std::cout<<std::setw(30)<<std::left<<"ReliefArenaNet: connecting to relief .... ";
    
    timeDelay = 0;
    timeWaiting = 0;
    
    
    bool res = false;
    do{
        sendRequest();
        res = reciveAnswer();
        
        if(res == true)
            timeDelay+=dt;
        timeWaiting+=dt;

    }while((timeDelay<1.5) && (timeWaiting<set.waitingConnectSec));
    
    if(res == false)
        std::cout<<"[FAIL ]"<<std::endl;
    else
        std::cout<<"[READY]"<<std::endl;
}
bool ReliefArenaNet::reciveAnswer()
{
    int bytes = 0;
    bytes = udpSocketRec->reciveFrom((uint8_t*)buf, sizeof(reliefAnswer));
        
    if(bytes>0)
    {
        memcpy((void*)&reliefAnswer,buf,sizeof(reliefAnswer));
        reliefAnswer.type = htonl(reliefAnswer.type);
        kin->dHRelief     = reliefAnswer.altitude;
        return true;
    }
    return false;
}

bool ReliefArenaNet::sendRequest()
{
    int bytes = 0;
    //! обнуление буфера
    memset((void*)buf,0,sizeof(buf));

    hRelief.id          = 1;
    hRelief.latitude    = kin->fi_geo * radToGrad;;
    hRelief.longitude   = kin->lam_geo * radToGrad;

    
    sizeBuf = 0;
    uint32_t *header = (uint32_t *)(&buf[0]);
    *header = ntohl((uint32_t)_GIS_MAP3D_GET_TERRAIN_ALTITUDE_);
    sizeBuf+=sizeof(uint32_t);
    
    memcpy((void*)&(buf[sizeBuf]),(void*)&hRelief,sizeof(hRelief));
    sizeBuf+=sizeof(hRelief);

    uint16_t *port = (uint16_t*)&buf[sizeBuf];
    *port = ntohs(set.portRecive);
    sizeBuf+=sizeof(uint16_t);

//    uint8_t *prot = (uint8_t*)&buf[sizeBuf];
//    *prot = 0;
//    sizeBuf+=sizeof(uint8_t);
       
    uint32_t *ip = (uint32_t*)&buf[sizeBuf];
    *ip = ipOwn;
    sizeBuf+=sizeof(uint32_t);

    bytes = udpSocket->sendTo(buf,sizeBuf);

    if(bytes>0)
        return true;
    return false;
      
}


