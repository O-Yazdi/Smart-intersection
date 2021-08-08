#ifndef SIMULATIONSCENE_H
#define SIMULATIONSCENE_H
#define SN_NS_SE_NW    0
#define SW_NE          1
#define WE_EW_EN_WS    2
#define ES_WN          3

#include "communicator.h"
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <omp.h>
#include <QtConcurrent/QtConcurrent>
#include "Entities/Vehicle/vehicle.h"
#include "Entities/trafficdetector.h"
#include "Entities/trafficcontroller.h"

class Communicator;

class SimulationScene: public QGraphicsScene
{

public:
    SimulationScene(QGraphicsScene *parent = nullptr);
    ~SimulationScene();
    uint getNumber(const region &x) const;
    QList<Vehicle *> getVehicle() const;
    QList<Vehicle *> getVehicle(const region &r) const;
    TrafficLight* getTrafficLight(const region &r) const;
    QList<TrafficLight *> getTrafficLight() const;
    QList<TrafficDetector *> getDetector() const;
    /*!
     * \brief Add vehicle to the simulation scene
     * \param vehicle = vehicle to be added
     */
    void addVehicle(Vehicle* vehicle);
    /*!
     * \brief add Detector to the simulation scene
     * \param detector to be added
     */
    void addDetector(TrafficDetector* detector);

    void addLight(TrafficLight* light);
    void removeVehicle(Vehicle* ve);
    void setGoThrough(bool x);
    void resetScene();
    void showTrafficLight();
    void HideTrafficLight();
    void showIntersectionPath(const bool &show);
    void showDetectors();
    void hideDetectors();
    void turnOffDetectors();
    void turnOnDetectors();
    void showVehiclesVision();
    void hideVehiclesVision();
    TrafficController *getController() const;
    void updateScene(const VEHICLEMETHOD &seeing);
    void turnOffInteraction();
    void turnOnInteraction();
    void setCsGreenLight(int CsNum);
    void TurnOffAllLights();

private:
    QList<Vehicle* > m_Vehicles;
    TrafficController *m_Controller;
    QGraphicsSvgItem *m_path;

    Communicator *comm;

//protected:
//    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // SIMULATIONSCENE_H
