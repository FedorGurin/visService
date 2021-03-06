#ifndef VIS_ARENA_NET_H
#define VIS_ARENA_NET_H

#include "visualProtocolArena.h"
#include "ICalculateElement.h"
#include "IEvent.h"
#include "globalData.h"
#include "templateUdpSocket.h"

typedef struct TSettingVisArena_
{   
    std::vector<uint16_t> port;
    std::vector<uint32_t> ipVis;
    std::vector<std::string> ipStr;
}TSettingVisArena;
#define LEN_BUF_AR 1048

struct SHelicopter
{
    struct
    {
        float Height;//Высота от поверхности, м
        struct
        {
            uint32_t Rpm_screw : 16;//! Кол-во оборотов для винта, об/мин
            uint32_t Light_on  : 1; //! Влючить источник света
            uint32_t Dustiness : 7; //! Тип поверхности, запыленность (0-асфальт,1-пыльный асфальт, 2-песок......)
            uint32_t reserve   : 8; //
        }w0;
        int32_t reserve1;//
        int32_t reserve2;//
    }b0;
};

typedef struct TSetupCloud_
{
    double latitude;    //Широта, гр
    double longitude;   //Долгота, гр
    float altitude;     //Высота нижней кромки, м
    float depth;        //Толщина, м
    float size;         //Радиус, км

    uint16_t cloudType;           //! тип облака
    uint8_t cloudDensity;         //! плотность облаков, %
    uint8_t precipitationType;    //! тип осадков
    uint8_t precipitationDensity; //! плотность осадков

}TSetupCloud;

typedef struct TSetupVisArena_
{
	uint8_t blockSending;
}TSetupVisArena;
//! Класс описывающий взаимодействие с КСУ
class  VisArenaNet:public ICalculateElement
{
public:
    VisArenaNet(uint32_t idClass,TSettingVisArena set_);

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
    
    bool sendPosCamera();
    bool sendCloud();
    bool sendFog();
    bool sendTime();

private:
    //! указатель на кинематику
    TKinematicAircraft *kin;
    TSettingVisArena    set;
    TSetupVisArena      setup;
    TSetupCloud         setupCloud, setupCloud_prev;
    SVisualTime         setupTime, setupTime_prev;
    SVisualFog          setupFog,  setupFog_prev;

    std::vector<UdpSocket *> udpSockets;
    SVisualCamera       camera;
    SVisualCloud        cloud;
    SVisualObjectHead     head;
    SVisualObjectPosition pos;
    SHelicopter           rot;
    double             *rpm100;
    uint8_t             buf[LEN_BUF_AR];
    uint16_t            sizeBuf;
};

#endif
