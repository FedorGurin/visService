#ifndef VIS_FG_NET_H
#define VIS_FG_NET_H

#include "net_fdm.hxx"
#include "ICalculateElement.h"
#include "IEvent.h"
#include "globalData.h"
#include "templateUdpSocket.h"

typedef struct TSettingVisFG_
{   
    std::vector<uint16_t> port;
    std::vector<uint32_t> ipVis;
    std::vector<std::string> ipStr;
}TSettingVisFG;

//! Класс описывающий взаимодействие с КСУ
class  VisFGNet:public ICalculateElement
{
public:
    VisFGNet(uint32_t idClass,TSettingVisFG set_);

    //! обобщенный интерфейс
    virtual void init();
    virtual void calculate();
    virtual void finite(){}

    //! отправление данных
    int send(void);
    //! получение данных
    int recive(void);
    //! заполнение данными
    void setData();
    //! получение данных
    void getData();
    //! отправить положение камеры
    bool sendPosCamera();
private:
    //! указатель на кинематику
    Solid *solid;
    TSettingVisFG    set;
    std::vector<UdpSocket *> udpSockets;
    //SVisualCamera       camera;
    //SVisualCloud        cloud;
    FGNetFDM            data;
};

#endif
