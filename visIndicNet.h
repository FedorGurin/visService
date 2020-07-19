#ifndef VIS_INDIC_NET_H
#define VIS_INDIC_NET_H

#include "visualProtocolArena.h"
#include "ICalculateElement.h"
#include "IEvent.h"
#include "globalData.h"
#include "templateUdpSocket.h"
#include "mfi141Pil.h"

typedef struct TSettingVisIndic_
{   
    uint16_t portMFI1;
    uint16_t portIM;
    uint32_t ipVisSKI;
}TSettingVisIndic;
#define LEN_BUF_MFI 2048


//! Класс описывающий взаимодействие c модулем индикации
class  VisIndicNet:public ICalculateElement
{
public:
    VisIndicNet(uint32_t idClass,TSettingVisIndic set_);

    //! обобщенный интерфейс
    virtual void init();
    virtual void calculate();
    virtual void finite(){}

    //! отправка данных в индикацию СИВВО
    bool sendMfi_1();
    bool sendIM_1();

private:
    //! указатель на кинематику
    TKinematicAircraft *kin;
    TSettingVisIndic    set;
    UdpSocket * udpSocketMFI;
    UdpSocket * udpSocketIM;

    uint8_t             buf1[LEN_BUF_MFI];
    uint16_t            sizeBuf1;
    uint8_t             buf2[LEN_BUF_MFI];
    uint16_t            sizeBuf2;

    SMfdPilResearchOutput *mfi_1;
    SMfdPilResearchOutput *mfi_2;
//    DataToStream *stream1;
//    DataToStream *stream2;

};

#endif
