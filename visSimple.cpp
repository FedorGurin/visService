#include "visSimple.h"
#include "platform.h"
#include <iostream>
#include "IMsg.h"
#include "globalNameID.h"

VisSimple::VisSimple(uint32_t idClass,TSettingVisSimple set_):ICalculateElement(idClass)
{
    set     = set_;
    listVis.clear();

    transTable.insert(std::pair<EGlobalProtoID,EVisSimpleCode>(ID_CONE_UPAZ         , E_3D_CONE));
    transTable.insert(std::pair<EGlobalProtoID,EVisSimpleCode>(ID_Proto_TankerIL78  , E_3D_IL76));
    transTable.insert(std::pair<EGlobalProtoID,EVisSimpleCode>(ID_Proto_Mig29       , E_3D_F15));
    transTable.insert(std::pair<EGlobalProtoID,EVisSimpleCode>(ID_Probe             , E_3D_CONE));

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

    //! указываем зависимости
    //bindTo(new TEventAddrStat(ID_SOLID_PTR,   (uintptr_t)ptrToSolid,    TEventAddrStat::GET_POINTER,this));

    //! указыем зависимости
    setFreq(ICalculateElement::Hz50);
    setStart();
}
bool VisSimple::bind()
{
    return ICalculateElement::bind();
}

void VisSimple::init()
{
    std::vector<TObjVisSimple* >::iterator it;
    for(it = listVis.begin();it != listVis.end();++it)
    {
        //! отпарвка положения камеры
        sendPosCamera(*it);
    }
    ICalculateElement::init();
}

bool VisSimple::sendPosCamera(TObjVisSimple *obj)
{
    if(obj->type == TObjVisSimple::E_OBJ_VEHICLE)
    {
        TVehicleVisSimple *vis = static_cast<TVehicleVisSimple* > (obj);

        dataVehicle.id     = vis->idObj;
        dataVehicle.code   = vis->code;
        dataVehicle.c_g    = vis->solid->c_g;

        dataVehicle.psi    = vis->solid->psi;
        dataVehicle.gamma  = vis->solid->gamma;
        dataVehicle.tan    = vis->solid->tan;

        dataVehicle.alfa   = vis->solid->alfa;
        dataVehicle.beta   = vis->solid->beta;

        //data.n_c    = solid->n_c;

        dataVehicle.unt    = vis->solid->unt;
        dataVehicle.fi     = vis->solid->fi;
        dataVehicle.v_g    = vis->solid->vg;
        dataVehicle.v_c    = vis->solid->vc;

        dataVehicle.vc     = vis->solid->Vc;
        dataVehicle.fi0_geo      = vis->solid->fi0_geo;
        dataVehicle.lam0_geo     = vis->solid->lam0_geo;

        dataVehicle.fi_geo     = vis->solid->fi_geo;
        dataVehicle.lam_geo     = vis->solid->lam_geo;

        dataVehicle.omega  = vis->solid->omega;

        request.compressed       = 0;
        request.sizeBuf          = SIZE_BUF_DEFAULT;
        request.err              = 0;
        request.head.typeRequest = PARAM_OBJ;
        request.head.size        = sizeof(TMRequest);
        request.head.id          = 0;
        request.head.dtTics      = 0;
        request.head.tics        = 0;

        //! копирование данных в буфер запроса
        memcpy((void*)request.buffer,(void*)&dataVehicle,sizeof(dataVehicle));
        for(int i = 0; i<udpSockets.size(); i++)
        {
            udpSockets[i]->sendTo((uint8_t*)&request,sizeof(TMRequest));
        }
        return true;
    }else if(obj->type == TObjVisSimple::E_OBJ_ARRAY)
    {
        //! отправка объекта состоящего из множества элементов
        return true;
    }
}
void VisSimple::calculate()
{
    std::vector<TObjVisSimple* >::iterator it;
    for(it = listVis.begin();it != listVis.end();++it)
    {
        sendPosCamera(*it);
    }
}

void VisSimple::eEvent(TEventModel *event)
{
    if(event->typeEvent() == TEventModel::E_MSG)
    {
        IMsg *msg = static_cast<IMsg*> (event);
        if(msg->typeMsg() == IMsg::E_REG_VIS)
        {
            MsgRegVis *msgRegVis = static_cast<MsgRegVis*> (msg);
            TEventAddrStat *e    = &(msgRegVis->e);
            bool prFind = false;

            std::vector<TObjVisSimple* >::iterator it;
            for(it = listVis.begin();it != listVis.end();++it)
            {
                if((*it)->idObj == msgRegVis->uid)
                    prFind = true;
            }
            if(prFind == false)
            {

                if(msgRegVis->typeVisObj == MsgRegVis::E_VIS_OBJ_VEHICLE)
                {
                    Solid *temp = 0;
                    e->setAddr(reinterpret_cast<uintptr_t> (&temp));
                    listVis.push_back(new TVehicleVisSimple(msgRegVis->uid,transTable[(EGlobalProtoID)msgRegVis->codeVis],temp));
                }else if(msgRegVis->typeVisObj == MsgRegVis::E_VIS_OBJ_VEHICLE)
                {
                    Solid *temp = 0;
                    e->setAddr(reinterpret_cast<uintptr_t> (&temp));
                    listVis.push_back(new TArrayVisSimple(msgRegVis->uid,transTable[(EGlobalProtoID)msgRegVis->codeVis],temp));
                }
            }


//            if(e->checkEvent(ID_CONNECT_TO_VISUAL,idObj) && e->operation == TEventAddrStat::SET_POINTER)
//            {

//                uintptr_t addr =(uintptr_t) *(TKinematicAircraft**)(e->addr);
//                data = (TSendVehicleVisSimple*)addr;

//                return;
//            }
        }
    }
    ICalculateElement::eEvent(event);
}
