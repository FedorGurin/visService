#ifndef VIS_SIMPLE_H
#define VIS_SIMPLE_H

#include "ICalculateElement.h"
#include "globalData.h"
#include "templateUdpSocket.h"
#include "globalNameID.h"
#include <vector>
#include <string>

#define SIZE_BUF_DEFAULT 2048
typedef struct TSendVehicleVisSimple_
{
    int id;
    int code;
    double time;
    glm::vec3 c_g;

    double psi;
    double gamma;
    double tan;

    double alfa;
    double beta;

    glm::vec3 n_c;
    double vc;

    double fi;
    double unt;

    double fi0_geo;
    double lam0_geo;

    double fi_geo;
    double lam_geo;

    glm::vec3 v_g;
    glm::vec3 v_c;
    glm::vec3 omega;
}TSendVehicleVisSimple;
typedef struct TSendArrayVisSimple_
{
    int id;
    int code;
    double time;

    //! координаты опорной точки
    double fi0_geo;
    double lam0_geo;

    //! кол-во элементов
    uint8_t num;

    typedef struct TGeoCoord_
    {
        double fi_geo;
        double lam_geo;
    }TGeoCoord;
    //! список объектов элемента
    TGeoCoord coord[256];
}TSendArrayVisSimple;

//! структура запроса для отправки/получения
typedef struct
{
    //! идентификатор программного модуля
    uint32_t id;
    //! тип запроса(
    uint32_t typeRequest;
    //! размер всего пакета
    uint32_t size;
    //! глобальное время(в тиках)
    uint32_t tics;
    //! предполагаемая задержка(в тиках) измеренная опытным путем
    uint32_t dtTics;
}THRequest;
//! структура для передачи всего буфера данных
typedef struct
{
    //! заголовок пакета
    THRequest head;
    //! признак сжатия буфера данных
    uint8_t compressed;
    //! код ошибки
    uint8_t err;
    //! размер структуры в буффере
    uint32_t sizeBuf;
    //! буффер с данными(размер массива указан в sizebuf)
    uint8_t buffer[SIZE_BUF_DEFAULT];
}TMRequest;

enum TypeCommand
{
    TC_RESET,/*перевод системы отображение в первоначальное состояние*/
    TC_CLEAR /*очистка траектории*/
};
//! структура для передачи команды в модуль
typedef struct
{
    //! заголовок пакета
    THRequest head;
    TypeCommand com;
}TCRequest;
//! для настройки сокетов
typedef struct TSettingVisSimple_
{
    std::vector<uint16_t> port;
    std::vector<uint32_t> ipVis;
    std::vector<std::string> ipStr;
}TSettingVisSimple;
class TObjVisSimple
{
public:
    enum TypeObj
    {
        E_OBJ_VEHICLE,
        E_OBJ_ARRAY
    };
    TObjVisSimple(int idObj_,int code_)
    {
        idObj = idObj_;
        code  = code_;
    }
    int code;
    int idObj;

    TypeObj type;
};

class TVehicleVisSimple : public TObjVisSimple
{
public:
    TVehicleVisSimple(int idObj_,int code_,Solid* solid_):TObjVisSimple(idObj_,code_)
    {
        solid = solid_;
        type  = TObjVisSimple::E_OBJ_VEHICLE;
    }
    Solid *solid;

};

class TArrayVisSimple : public TObjVisSimple
{
public:
    TArrayVisSimple(int idObj_,int code_,Solid* solid_):TObjVisSimple(idObj_,code_)
    {
        solid = solid_;
        type  = TObjVisSimple::E_OBJ_ARRAY;
    }
    Solid *solid;

};



//! класс для работы с системой визуализацией 3D World
class VisSimple:public ICalculateElement
{
public:
    VisSimple(uint32_t idClass,TSettingVisSimple set_);
    virtual bool bind();
    virtual void init();
    virtual void calculate();
    virtual void finite(){}
    virtual void eEvent(TEventModel *event);

    enum EVisSimpleCode
    {
        E_3D_CONE = 105,
        E_3D_F15  = 101,
        E_3D_IL76 = 102,
        E_3D_PROBE= 103
    };
    //! отправить положение камеры
    bool sendPosCamera(TObjVisSimple *vis);

    enum TTypeHead
    {
        PARAM_OBJ       = 0,
        PARAM_ARRAY     = 4,
        INFO_FLIGHT_OBJ = 1,
        COMMAND         = 2,
        CONTROL_STICK   = 3    /*управление ручкой указанными объектом*/
    };
private:
    //! запрос данных
    TMRequest request;
    //! список объектов, которые модуль визуализации должен отправлять
    std::vector<TObjVisSimple *> listVis;
    //! промежуточные объекты для отправки данных
    TSendVehicleVisSimple dataVehicle;
    TSendArrayVisSimple   dataArray;
    //! настройки модуля
    TSettingVisSimple    set;
    //! список рассылаемых сокетов
    std::vector<UdpSocket *> udpSockets;
    //! трансляция кодов(из внутренних в систему визуализацию
    std::map<EGlobalProtoID,EVisSimpleCode> transTable;
};

#endif // CONNECTTOVISUAL_H
