#ifndef _VISUAL_PROTOCOL_ARENA_H_
#define _VISUAL_PROTOCOL_ARENA_H_

//#include <QRectF>
//#include <QStringList>
//#include <QVector>
//#include <QDataStream>
//#include <qmath.h>
//#include "class_enum.h"
#include "platform.h"
#ifdef VXWORKS_PLATFORM
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif
//Идентификаторы пакетов управления системой визуализации
enum{_GIS_MAP3D_CLOSE_										=    0,
		 _GIS_MAP3D_SET_CAMERA_								= 1000,//SVisualCamera + (SVisualObjectHead + SVisualObjectPosition)[]
		 _GIS_MAP3D_SET_SCREEN_								= 1001,//SVisualScreen
		 _GIS_MAP3D_SET_TIME_									= 1002,//SVisualTime
		 _GIS_MAP3D_SET_ATMOSPHERE_						= 1003,//SVisualAtmosphere
		 _GIS_MAP3D_SET_WIND_									= 1009,//SVisualWind[]
		 _GIS_MAP3D_SET_CLOUD_								= 1010,//SVisualCloud[]
		 _GIS_MAP3D_SET_FOG_									= 1011,//SVisualFog[]
		 _GIS_MAP3D_SET_EXPLOSION_						= 1012,//SVisualExplosion
		 _GIS_MAP3D_SET_FIRE_									= 1013,//SVisualFire
		 _GIS_MAP3D_SET_OBJECT_ANIMATION_			= 1014,//SVisualObjectCoreHead + QByteArray
		 _GIS_MAP3D_SET_OBJECT_DAMAGE_				= 1015,//quint64
		 _GIS_MAP3D_SET_OBJECT_LABEL_					= 1016,//quint64 + quint32 + QString[]
		 _GIS_MAP3D_SET_OBJECT_TRACK_					= 1017,//quint64 + SVisualObjectTrackPoint[] + SVisualObjectTrackSettings
		 _GIS_MAP3D_SET_TARGET_								= 1018,//SVisualTarget + (SVisualObjectHead + SVisualObjectPosition)[]
		 _GIS_MAP3D_SET_OBJECTS_							= 1019,//(SVisualObjectHead + SVisualObjectPosition)[]
 		 _GIS_MAP3D_SET_VIEW_ANGLE_						= 1020,//SVisualViewAngle
		 _GIS_MAP3D_SET_MODE_									= 1021,//SVisualMode
		 _GIS_MAP3D_SET_EVENT_LABEL_					= 1022,//SVisualPoint + quint32 + QString[]
		 _GIS_MAP3D_SET_SYMBOL_LINE_					= 1023,//(SVisualPoint[] + quint32 + QString)[]
		 _GIS_MAP3D_SET_SYMBOL_AREA_					= 1024,//(SVisualPoint[] + quint32 + QString)[]
		 _GIS_MAP3D_SET_SYMBOL_VOLUME_				= 1025,//(SVisualPoint[] + quint32 + QString)[]
		 _GIS_MAP3D_CLEAN_										= 1030,//SVisualClean
		 _GIS_MAP3D_GET_READINESS_						= 1100,
		 _GIS_MAP3D_GET_TERRAIN_ALTITUDE_			= 1101,//SVisualTerrainAltitudeRequest + quint16 + QHostAddress
		 _GIS_MAP3D_GET_TERRAIN_INTERSECTION_	= 1102 //SVisualTerrainIntersectionRequest + quint16 + QHostAddress
		};

//Идентификаторы пакетов управления изображением индикаторов
enum{_GIS_INDICATOR_CONTROL_MAP2D_	= 2000,//SIndicatorMap2D
		 _GIS_INDICATOR_CONTROL_RADAR_	= 2001,//SIndicatorRadar + (SVisualObjectHead + SVisualObjectPosition)[]
		 _GIS_INDICATOR_CONTROL_TV_			= 2002,//uint8 + SIndicatorTV[] + QByteArray[]
		 _GIS_INDICATOR_CONTROL_METEO_	= 2003 //SIndicatorMeteo
		};

//Идентификаторы пакетов управления картой
enum{_GIS_MAP2D_SET_EYE_			= 3000,//SMap2DEye
		 _GIS_MAP2D_SET_ITEM_			= 3001,//QVector<QString + SMap2DItem>
		 _GIS_MAP2D_REMOVE_ITEM_	= 3002,//QVector<QString>
		 _GIS_MAP2D_SET_POLY_			= 3003,//QVector<QString + SMap2DPolyline>
		 _GIS_MAP2D_REMOVE_POLY_	= 3004,//QVector<QString + bool>
		 _GIS_MAP2D_SET_PATH_			= 3005,//QVector<QString + SMap2DPath>
		 _GIS_MAP2D_REMOVE_PATH_	= 3006,//QVector<QString>
		 _GIS_MAP2D_REMOVE_ALL_		= 3007,//
		 _GIS_MAP2D_SET_SENSOR_		= 3008,//bool
		 _GIS_MAP2D_SET_ANCHOR_		= 3009 //QString
		};

//Cигналы от карты
enum{_GIS_MAP2D_LCLICK_ITEM_			= 3100,//Клик левой кнопкой мыши на символе
		 _GIS_MAP2D_LMOVE_ITEM_				= 3101,//Перемещение символа левой кнопкой мыши
		 _GIS_MAP2D_LDBLCLICK_ITEM_		= 3102,//Двойной клик левой кнопкой мыши на символе
		 _GIS_MAP2D_RCLICK_ITEM_			= 3103,//Клик правой кнопкой мыши на символе
		 _GIS_MAP2D_RCLICK_EMPTY_			= 3104 //Клик правой кнопкой мыши в пустом месте
		};

//Идентификаторы пакетов запросов к базе данных
enum{_GIS_DATABASE_GET_CLASS3D_		= 4000,//
		 _GIS_DATABASE_GET_AERONAV_		= 4001,//
		 _GIS_DATABASE_GET_COORD3D_		= 4002 //
		};

//Пакет управления режимом отображения
struct SVisualMode
{
	union{
		struct{
			uint32_t mode : 3;//Режим отображения, EMode 
			uint32_t cameraMode : 2;//Режимы работы камеры ECameraMode
			uint32_t hideAtmosphere : 1;//Отключить атмосферу (воздушная перспектива)
			uint32_t hideSky : 1;//Отключить атмосферу (небо)
			uint32_t hideShadows : 1;//Отключить тени
			uint32_t hideStars : 1;//Отключить Солнце, Луну и звезды
			uint32_t hideClouds : 1;//Отключить облачность, туман и осадки
			uint32_t hideFires : 1;//Отключить форсажи, дымы, взрывы и пожары
			uint32_t hideLights : 1;//Отключить световую сигнализацию, фары, фонари и окна
			uint32_t hideObjects : 1;//Отключить динамические объекты
			uint32_t hideSymbols : 1;//Отключить символьные объекты (зоны, маршруты, нав. точки)
			uint32_t reserve : 18;
		}signs;
		uint32_t _signs;
	};
	uint64_t linkName;//Имя объекта слежения

	enum EMode{_Mode_Realistic_ = 0,//реалистичный
						 _Mode_Infrared_	= 1,//тепловой
						 _Mode_Tactic_		= 7 //тактический
						};

	enum ECameraMode{_Camera_Free_		= 0,//свободный
									 _Camera_Central_ = 1,//центральный
									 _Camera_Linked_	= 2 //следящий("глаз бога")
									};

//	SVisualMode():_signs(0),linkName(0){}
//	SVisualMode(const SVisualMode& m):_signs(m._signs),linkName(m.linkName){}
};

//Пакет управления режимом очистки
struct SVisualClean
{
	union{
		struct{
			uint32_t resetWind : 1;//Ветры
			uint32_t resetCloud : 1;//Облака
			uint32_t resetFog : 1;//Туманы
			uint32_t resetExplosion : 1;//Взрывы
			uint32_t resetFire : 1;//Огни/дымы
			uint32_t resetObject : 1;//Объекты
			uint32_t resetObjectLabel : 1;//Подписи объектов
			uint32_t resetObjectTrack : 1;//Траектории объектов
			uint32_t resetEventLabel : 1;//Подписи событий
			uint32_t resetSymbolLine : 1;//Символьные линии
			uint32_t resetSymbolArea : 1;//Символьные площади
			uint32_t resetSymbolVolume : 1;//Символьные объемы
			uint32_t reserve : 20;
		}signs;
		uint32_t _signs;
	};

//	SVisualClean():_signs(0){}
//	SVisualClean(const SVisualClean& с):_signs(с._signs){}
};

//Пакет управления положением глаза наблюдателя
struct SVisualCamera
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м
	float head;//Курс, гр
	float pitch;//Тангаж, гр
	float roll;//Крен, гр
	float vx;//Проекция скорости по оси X, м/с
	float vy;//Проекция скорости по оси Y, м/с
	float vz;//Проекция скорости по оси Z, м/с

//	SVisualCamera():latitude(0.0),longitude(0.0),altitude(0.0f),head(0.0f),pitch(0.0f),roll(0.0f),vx(0.0f),vy(0.0f),vz(0.0f){}
//	SVisualCamera(const SVisualCamera& c):latitude(c.latitude),longitude(c.longitude),altitude(c.altitude),head(c.head),pitch(c.pitch),roll(c.roll),vx(c.vx),vy(c.vy),vz(c.vz){}
};

//Пакет управления направлением зрения наблюдателя
struct SVisualViewAngle
{
	float ax;//Поворот по оси X, гр
	float ay;//Поворот по оси Y, гр
	float az;//Поворот по оси Z, гр

//	SVisualViewAngle():ax(0.0f),ay(0.0f),az(0.0f){}
//	SVisualViewAngle(const SVisualViewAngle& va):ax(va.ax),ay(va.ay),az(va.az){}
};

//Пакет управления положением объекта наблюдения
struct SVisualTarget
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м
	double targetLatitude;//Широта цели, гр
	double targetLongitude;//Долгота цели, гр
	float targetAltitude;//Высота цели, м
	float vx;//Проекция скорости по оси X, м/с
	float vy;//Проекция скорости по оси Y, м/с
	float vz;//Проекция скорости по оси Z, м/с

//	SVisualTarget():latitude(0.0),longitude(0.0),altitude(0.0f),targetLatitude(0.0),targetLongitude(0.0),targetAltitude(0.0f),vx(0.0f),vy(0.0f),vz(0.0f){}
//	SVisualTarget(const SVisualTarget& t):latitude(t.latitude),longitude(t.longitude),altitude(t.altitude),targetLatitude(t.targetLatitude),targetLongitude(t.targetLongitude),targetAltitude(t.targetAltitude),vx(t.vx),vy(t.vy),vz(t.vz){}
};

//Пакет управления параметрами экрана
struct SVisualScreen
{
	float hfov;//Горизонтальный угол обзора, гр
	float vfov;//Вертикальный угол обзора, гр
	union{
		struct{			
			uint32_t focus : 1;//Фокусировка
			uint32_t reserve : 31;
		}signs;
		uint32_t _signs;
	};

//	SVisualScreen():hfov(0.0f),vfov(0.0f),_signs(0){}
//	SVisualScreen(const SVisualScreen& s):hfov(s.hfov),vfov(s.vfov),_signs(s._signs){}
};

//Пакет управления текущим временем на 0-ом меридиане (UTC)
struct SVisualTime
{
	uint16_t year;//Год
	uint8_t month;//Месяц
	uint8_t day;//День
	uint8_t hour;//Час
	uint8_t minute;//Минута
	float second;//Секунда

//	SVisualTime():year(0),month(0),day(0),hour(0),minute(0),second(0.0f){}
//	SVisualTime(const SVisualTime& t):year(t.year),month(t.month),day(t.day),hour(t.hour),minute(t.minute),second(t.second){}
};

struct SVisualAtmosphere
{
	float range;//Метеорологическая дальность видимости, км
	float latitude;//Широта зоны интереса, гр
	uint8_t month;//Месяц

//	SVisualAtmosphere():range(300.0f),latitude(45.0f),month(0){}
//	SVisualAtmosphere(const SVisualAtmosphere& a):range(a.range),latitude(a.latitude),month(a.month){}
};

//Пакет управления ветрами
struct SVisualWind
{
	float altitude;//Высота, м
	float speed;//Скорость, м/с
	float direction;//Направление, гр
	float sigma;//СКО турбулентности
  float scale;//Масштаб турбулентности

//	SVisualWind():altitude(0.0f),speed(0.0f),direction(0.0f),sigma(0.0f),scale(0.0f){}
//	SVisualWind(const SVisualWind& w):altitude(w.altitude),speed(w.speed),direction(w.direction),sigma(w.sigma),scale(w.scale){}
};

//Пакет управления облаками
struct SVisualCloud
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота нижней кромки, м
	float depth;//Толщина, м
	float size;//Радиус, км
	union{
		struct{			
		    uint32_t cloudType : 10;//Тип облаков
			uint32_t cloudDensity : 7;//Плотность облаков, %
			uint32_t precipitationType : 3;//Тип осадков
			uint32_t precipitationDensity : 7;//Плотность осадков, %
			uint32_t reserve : 5;
		}signs;
		uint32_t _signs;
	};

//	SVisualCloud():latitude(0.0),longitude(0.0),altitude(0.0f),depth(0.0f),size(0.0f),_signs(0){}
//	SVisualCloud(const SVisualCloud& c):latitude(c.latitude),longitude(c.longitude),altitude(c.altitude),depth(c.depth),size(c.size),_signs(c._signs){}
};

//Пакет управления туманом
struct SVisualFog
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float depth;//Толщина, м
	float size;//Протяженность, км
	float range;//Локальная дальность видимости, м

//	SVisualFog():latitude(0.0),longitude(0.0),depth(0.0f),size(0.0f),range(0.0f){}
//	SVisualFog(const SVisualFog& f):latitude(f.latitude),longitude(f.longitude),depth(f.depth),size(f.size),range(f.range){}
};

//Пакет управления врывом
struct SVisualExplosion
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м
	union{//Внешний вид (характеристика)
		struct{
		    uint16_t code;
		    uint16_t value;
		}view;
		uint32_t _view;
	};

//	SVisualExplosion():latitude(0.0),longitude(0.0),altitude(0.0f),_view(0){}
//	SVisualExplosion(const SVisualExplosion& e):latitude(e.latitude),longitude(e.longitude),altitude(e.altitude),_view(e._view){}
};

//Пакет управления огнем/дымом
struct SVisualFire
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float duration;//Длительность, с
	union{//Внешний вид (характеристика)
		struct{
			uint16_t code;
			uint16_t value;
		}view;
		uint32_t _view;
	};

//	SVisualFire():latitude(0.0),longitude(0.0),duration(0.0f),_view(0){}
//	SVisualFire(const SVisualFire& f):latitude(f.latitude),longitude(f.longitude),duration(f.duration),_view(f._view){}
};

//Заголовок пакета управления динамическим объектом
struct SVisualObjectHead
{
    uint32_t type;//Тип (класс)
	union{//Внешний вид (характеристика)
		struct{
		    uint16_t code;
			uint16_t value;
		}view;
		uint32_t _view;
	};
	uint64_t name;//Имя (уникальный ID)
	uint32_t timeout;//Продолжительность отображения с текущими параметрами, мс

//	SVisualObjectHead():type(0),_view(0),name(0),timeout(0){}
//	SVisualObjectHead(const SVisualObjectHead& h):type(h.type),_view(h._view),name(h.name),timeout(h.timeout){}
};

//Пакет управления положением динамического объекта
struct SVisualObjectPosition
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м
	float head;//Курс, гр
	float pitch;//Тангаж, гр
	float roll;//Крен, гр
	union{
		struct{		
		    uint32_t age : 28;//Время от начала создания, мс
			uint32_t magnitic : 1;//Магнитный курс
			uint32_t onwater : 1;//На поверхности воды
			uint32_t onground : 1;//На поверхности земли
			uint32_t groundcollision : 1;//Разрушение при столкновении с поверхностью
		}signs;
		uint32_t _signs;
	};
	float vx;//Проекция скорости по оси X, м/с
	float vy;//Проекция скорости по оси Y, м/с
	float vz;//Проекция скорости по оси Z, м/с

//	SVisualObjectPosition():latitude(0.0),longitude(0.0),altitude(0.0f),head(0.0f),pitch(0.0f),roll(0.0f),_signs(0),vx(0.0f),vy(0.0f),vz(0.0f){}
//	SVisualObjectPosition(const SVisualObjectPosition& p):latitude(p.latitude),longitude(p.longitude),altitude(p.altitude),head(p.head),pitch(p.pitch),roll(p.roll),_signs(p._signs),vx(p.vx),vy(p.vy),vz(p.vz){}
};

//Структура точки траектории динамического объекта
struct SVisualObjectTrackPoint
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м
	float roll;//Крен, гр
	uint32_t age;//Время от начала создания, мс

//	SVisualObjectTrackPoint():latitude(0.0),longitude(0.0),altitude(0.0f),roll(0.0f),age(0){}
//	SVisualObjectTrackPoint(const SVisualObjectTrackPoint& p):latitude(p.latitude),longitude(p.longitude),altitude(p.altitude),roll(p.roll),age(p.age){}
};

//Структура параметров траектории динамического объекта
struct SVisualObjectTrackSettings
{
    uint32_t color;
	union{
		struct{
		    uint32_t hideObject : 1;//Отключить отображение объекта
			uint32_t trackType : 3;//Тип отображения траектории, ETrackType
			uint32_t showLabel : 1;//Включить отображение подписи
			uint32_t trackRange : 1;//Использовать временные пределы
			uint32_t backTime : 13;//Протяженность траектории сзади объекта, с
			uint32_t forwardTime : 13;//Протяженность траектории впереди объекта, с	
		}signs;
		uint32_t _signs;
	};
	float objectAngle;//Угловой размер объекта минимальный, гр

	enum ETrackType{_Track_No_		= 0,//нет
									_Track_Line_	= 1,//линия
									_Track_Band_	= 2,//лента
									_Track_Wall_	= 3 //забор
								 };
	
//	SVisualObjectTrackSettings():color(0),_signs(0),objectAngle(2.5f){ signs.trackType = _Track_Line_; }
//	SVisualObjectTrackSettings(const SVisualObjectTrackSettings& s):color(s.color),_signs(s._signs),objectAngle(s.objectAngle){}
};

struct SVisualPoint
{
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м

//	SVisualPoint():latitude(0.0),longitude(0.0),altitude(0.0f){}
//	SVisualPoint(double lat, double lon, float alt):latitude(lat), longitude(lon), altitude(alt){}
//	SVisualPoint(const SVisualPoint& p):latitude(p.latitude),longitude(p.longitude),altitude(p.altitude){}
};

//Пакет запроса высоты рельефа
struct SVisualTerrainAltitudeRequest
{
    uint32_t id;//Уникальный ID
	double latitude;//Широта, гр
	double longitude;//Долгота, гр

//	SVisualTerrainAltitudeRequest():id(0),latitude(0.0),longitude(0.0){}
//	SVisualTerrainAltitudeRequest(const SVisualTerrainAltitudeRequest& r):id(r.id),latitude(r.latitude),longitude(r.longitude){}
};

//Пакет запроса пересечения рельефа
struct SVisualTerrainIntersectionRequest
{
    uint32_t type;
    uint32_t id;//Уникальный ID
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м
	float head;//Курс, гр
	float pitch;//Тангаж, гр

//	SVisualTerrainIntersectionRequest():id(0),latitude(0.0),longitude(0.0),altitude(0.0f),head(0.0f),pitch(0.0f){}
//	SVisualTerrainIntersectionRequest(const SVisualTerrainIntersectionRequest& r):id(r.id),latitude(r.latitude),longitude(r.longitude),altitude(r.altitude),head(r.head),pitch(r.pitch){}
};

//Пакет ответа на запрос высоты/пересечения рельефа
struct SVisualTerrainAnswer
{
    uint32_t type;
	uint32_t id;//Уникальный ID	
	double latitude;//Широта, гр
	double longitude;//Долгота, гр
	float altitude;//Высота, м
	uint8_t surface;//Поверхность
	uint8_t result;//Результат

//	SVisualTerrainAnswer():id(0),latitude(0.0),longitude(0.0),altitude(0.0f),surface(0),result(0){}
//	SVisualTerrainAnswer(const SVisualTerrainAnswer& a):id(a.id),latitude(a.latitude),longitude(a.longitude),altitude(a.altitude),surface(a.surface),result(a.result){}
};



#ifdef VXWORKS_PLATFORM
#pragma pack(0)
#else
#pragma pack(pop)
#endif

#endif
