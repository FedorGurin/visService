#ifndef DATA_TO_RP_H
#define DATA_TO_RP_H

#include "ICalculateElement.h"
#include "globalData.h"
#include "templateUdpSocket.h"
#include "globalNameID.h"
#include "./s226/isrp5_3.h"
#include <vector>
#include <string>

//! для настройки сокетов
typedef struct TSettingDataToRP_
{
    uint16_t    port;
    uint32_t    ipVis;
    std::string ipStr;
}TSettingDataToRP;

//! класс для выдачи данных в АРМ РП
class DataToRP:public ICalculateElement
{
public:
    DataToRP(uint32_t idClass,TSettingDataToRP set_);
    virtual bool bind();
    virtual void init();
    virtual void calculate();
    virtual void finite(){}
private:
    //! сокет для отправки в АРМ РП
    UdpSocket*   udpSocket;
    TWorkISRP5_3 *work;
    TSettingDataToRP set;
};

#endif // CONNECTTOVISUAL_H
