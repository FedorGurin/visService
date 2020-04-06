#include "dataToRP.h"
#include "platform.h"
#include <iostream>
#include "IMsg.h"
#include "globalNameID.h"

DataToRP::DataToRP(uint32_t idClass,TSettingDataToRP set_):ICalculateElement(idClass)
{
    set     = set_;
    udpSocket = new UdpSocket;
    udpSocket->init(set.port, set.ipVis);
    
    work = 0;

    //! указываем зависимости
    bindTo(new TEventAddrStat(ID_ISRP_WORK,   (uintptr_t)&work,    TEventAddrStat::GET_POINTER,this));

    //! указыем зависимости
    setFreq(ICalculateElement::Hz50);
    setStart();
}
bool DataToRP::bind()
{
    return ICalculateElement::bind();
}

void DataToRP::init()
{
    ICalculateElement::init();
}


void DataToRP::calculate()
{
    if(work!=0)
    udpSocket->sendTo((uint8_t*)work,sizeof(TWorkISRP5_3));
}


