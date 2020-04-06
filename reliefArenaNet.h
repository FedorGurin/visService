#ifndef RELIEF_ARENA_NET_H
#define RELIEF_ARENA_NET_H

#include "visualProtocolArena.h"
#include "ICalculateElement.h"
#include "IEvent.h"
#include "globalData.h"
#include "templateUdpSocket.h"

typedef struct TSettingReliefArena_
{
    TSettingReliefArena_()
    {
        waitingConnectSec = 15.;
        dtCheckConnectMs  = 250.0;
    }

    uint16_t portSend;            //! порт отправки пакетов
    uint16_t portRecive;          //! порт приема пакетов
    uint32_t ipVis;               //! ip адрес узла СВ
    uint32_t ipOwn;               //! ip адрес текущего узла
    float    dtCheckConnectMs;    //! проверка соединения каждые X мсек
    float    waitingConnectSec;   //! время ожидания для подключения к СВ
}TSettingReliefArena;
//! Класс описывающий взаимодействие с рельефом СВ АРЕНА
class  ReliefArenaNet:public ICalculateElement
{
public:
    ReliefArenaNet(uint32_t idClass,TSettingReliefArena set_);

    //! обобщенный интерфейс
    virtual void init();
    virtual void calculate();
    virtual void finite(){}

    bool sendRequest();
    bool reciveAnswer();
  
    //! отправление данных
    int send(void);
    //! получение данных
    int recive(void);
    //! заполнение данными
    void setData();
    //! получение данных
    void getData();
    //! ожидание ответа от рельефа
    void waitingReliefAns();
private:
    //! счетчики приема/передачи
    long countSending;
    long countReciving;
    //! указатель на кинематику
    TKinematicAircraft *kin;
    SVisualTerrainAltitudeRequest hRelief;
    SVisualTerrainIntersectionRequest iterRelief;
    SVisualTerrainAnswer reliefAnswer;
    uint8_t buf[1048];
    uint16_t sizeBuf;
    UdpSocket *udpSocket;
    UdpSocket *udpSocketRec;
    TSettingReliefArena set;
    uint32_t ipOwn;
    float timeDelay;
    float timeWaiting;


};

#endif
