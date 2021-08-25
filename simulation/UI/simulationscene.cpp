#include "simulationscene.h"
#include <thread>
#include<iostream>
int ind =0;
int eerr = 0;
SimulationScene::SimulationScene(QGraphicsScene *parent):QGraphicsScene (parent)//, comm(this)
{
    //Add Road and Background to scene
    //QGraphicsPixmapItem *m_picture = new QGraphicsPixmapItem(QPixmap(":/image/Image/road-image.png")/*.scaled (600,600)*/);
    QGraphicsSvgItem *m_terrain = new QGraphicsSvgItem(":/image/Image/terrain.svg");
    m_terrain->setFlag(QGraphicsItem::ItemIsMovable,false);
    m_terrain->setFlag(QGraphicsItem::ItemIsSelectable,false);
    m_path = new QGraphicsSvgItem(":/image/Image/road-path.svg");
    m_path->setFlag(QGraphicsItem::ItemIsMovable,false);
    m_path->setFlag(QGraphicsItem::ItemIsSelectable,false);
    //m_terrain->moveBy (-60,0);
    //m_picture->moveBy(30,0);
    m_path->moveBy (15,-15);
    m_path->setScale (0.7);
    m_path->setZValue (-1);
    m_path->setOpacity(0);
    //m_picture->setZValue (-2);
    m_terrain->setZValue (-3);
    m_terrain->moveBy(-70,5);
    this->addItem (m_terrain);
    this->addItem (m_path);

    setItemIndexMethod(QGraphicsScene::NoIndex);

    setBackgroundBrush (Qt::white);
    m_Controller = new TrafficController;
    this->addItem(m_Controller);

    comm = new Communicator(*this);
    try
    {
        if(ind != 0)
        {
            comm->connect("127.0.0.1", 8826);
            std::thread (&Communicator::startConnection, comm).detach();
        }
        ind++;

    }

    catch(...)
    {
        qDebug()<<eerr;
    }
}

SimulationScene::~SimulationScene()
{
    if (comm != nullptr)
    {
        delete comm;
        comm = nullptr;
    }
}

uint SimulationScene::getNumber(const region &x) const
{
    uint size = 0;
    QList<QGraphicsItem *> v = this->items();
    QList<Vehicle *> p;
    for(int i = 0 ; i < v.size() ; ++i){
        if(dynamic_cast<Vehicle *>(v.at(i))){
            p.append(dynamic_cast<Vehicle *>(v.at(i)));
        }
    }
    for(int i = 0 ; i < p.size() ; ++i){
        if(p.at(i)->getRegion() == x){
            size++;
        }
    }
    return size;
}

QList<Vehicle *> SimulationScene::getVehicle() const
{
    return m_Vehicles;
}

QList<Vehicle *> SimulationScene::getVehicle(const region &r) const
{
    QList<Vehicle* > ve;
    for(size_t i = 0 ; i < m_Vehicles.size() ; ++i ){
        if(m_Vehicles.at(i)->getRegion() == r){
            ve.append(m_Vehicles.at(i));
        }
    }
    return ve;
}

QList<TrafficDetector *> SimulationScene::getDetector() const
{
    return m_Controller->getDetector();
}

void SimulationScene::addVehicle(Vehicle *vehicle)
{
    m_Vehicles.append(vehicle);
    this->addItem(vehicle);
}


void SimulationScene::removeVehicle(Vehicle *ve)
{
    m_Vehicles.removeOne(ve);
    this->removeItem(ve);
    delete ve;
}

TrafficLight * SimulationScene::getTrafficLight(const region &r) const
{
    return m_Controller->getTrafficLight(r);
}

QList<TrafficLight *> SimulationScene::getTrafficLight() const
{
    return m_Controller->getTraffic_light();
}


void SimulationScene::setGoThrough(bool x)
{
    if(x){
        for(int i = 0 ; i < m_Vehicles.size() ; ++i){
            m_Vehicles.at(i)->setMode(VEHICLEMETHOD::GO_THROUGH);
        }
    }else{
        for(int i = 0 ; i < getVehicle().size() ; ++i){
            m_Vehicles.at(i)->setMode(VEHICLEMETHOD::SIGHTSEEING);
        }
    }
}

void SimulationScene::resetScene()
{
    for(int i = 0 ; i < this->getVehicle().size() ; i++){
       this->removeItem(this->getVehicle().at(i));
       //delete this->getVehicle().at(i);
    }
    for(int i = 0 ; i < this->getTrafficLight().size() ; i++){
        this->removeItem(this->getTrafficLight().at(i));
    }
}

void SimulationScene::showTrafficLight()
{
    for(int i = 0 ; i < m_Controller->getTraffic_light().size() ; ++i){
        m_Controller->getTraffic_light().at(i)->setMode(TRAFFICMODE::HAS_SIGNAL);
        this->addItem(m_Controller->getTraffic_light().at(i));
    }
}

void SimulationScene::HideTrafficLight()
{
    for(int i = 0 ; i < m_Controller->getTraffic_light().size() ; ++i){
        m_Controller->getTraffic_light().at(i)->setMode(TRAFFICMODE::NO_SIGNAL);
        this->removeItem(m_Controller->getTraffic_light().at(i));
    }
}

void SimulationScene::showIntersectionPath(const bool &show)
{
    if(show){
        m_path->setOpacity(100);
    }else{
        m_path->setOpacity(0);
    }
}

void SimulationScene::showDetectors()
{
    m_Controller->showDetector();
}

void SimulationScene::hideDetectors()
{
    m_Controller->hideDetector();
}

void SimulationScene::turnOffDetectors()
{
    m_Controller->turnOffDetector();
}

void SimulationScene::turnOnDetectors()
{
    m_Controller->turnOnDetector();
}

void SimulationScene::showVehiclesVision()
{
    for(int i = 0 ; i < m_Vehicles.size() ; ++i){
        m_Vehicles.at(i)->turnOnSightSeeing();
    }
}

void SimulationScene::hideVehiclesVision()
{
    for(int i = 0 ; i < m_Vehicles.size() ; ++i){
        m_Vehicles.at(i)->turnOffSightSeeing();
    }
}

TrafficController *SimulationScene::getController() const
{
    return m_Controller;
}

void SimulationScene::updateScene(const VEHICLEMETHOD &seeing)
{
#if PARALLEL_OMP

    omp_set_num_threads(4);
    #pragma omp parallel private(m_Vehicles)
    {

        for(auto i = m_Vehicles.begin() ; i != m_Vehicles.end() ; ++i){
            #pragma omp single nowait
            {
                (*i)->update(seeing);

            }
        }
    }
    #pragma omp parallel for
    for(size_t i = 0 ; i < m_Vehicles.size() ; ++i ){
        if(m_Vehicles.at(i)->isDeletable()){
            removeVehicle(m_Vehicles.at(i));
        }
    }
#elif PARALLEL_CONCURRENT
    for(int i = 0; i < m_Vehicles.size() ; ++i){

        QFuture<void> f = QtConcurrent::run(m_Vehicles.at(i),&Vehicle::update,seeing);

    }
#else
    advance();
    for(size_t i = 0 ; i < m_Vehicles.size() ; ++i ){
        if(m_Vehicles.at(i)->isDeletable()){
            removeVehicle(m_Vehicles.at(i));
        }
    }
#endif
}

void SimulationScene::turnOffInteraction()
{
    for(int i = 0 ; i < m_Vehicles.size() ; ++i){
        m_Vehicles.at(i)->turnOffInteraction();
    }
    m_Controller->turnOffLightInteraction();


}

void SimulationScene::turnOnInteraction()
{
    for(int i = 0 ; i < m_Vehicles.size() ; ++i){
        m_Vehicles.at(i)->turnOnInteraction();
    }
    m_Controller->turnOnLightInteraction();
}

void SimulationScene::setCsGreenLight(int CsNum)
{
    int currentCsGreen = getCurrentCsGreen();

    if(currentCsGreen==CsNum)
        return;

    switch (currentCsGreen) {
    case 0:
        m_Controller->getTraffic_light().at(0)->getLight()->at(1)->turnOff();
        m_Controller->getTraffic_light().at(2)->getLight()->at(1)->turnOff();

        m_Controller->getTraffic_light().at(0)->getLight()->at(2)->turnOn();
        m_Controller->getTraffic_light().at(2)->getLight()->at(2)->turnOn();

        break;
    case 1:
        m_Controller->getTraffic_light().at(0)->getLight()->at(0)->turnOff();
        m_Controller->getTraffic_light().at(2)->getLight()->at(0)->turnOff();

        m_Controller->getTraffic_light().at(0)->getLight()->at(2)->turnOn();
        m_Controller->getTraffic_light().at(2)->getLight()->at(2)->turnOn();

        break;
    case 2:
        m_Controller->getTraffic_light().at(1)->getLight()->at(1)->turnOff();
        m_Controller->getTraffic_light().at(3)->getLight()->at(1)->turnOff();

        m_Controller->getTraffic_light().at(1)->getLight()->at(2)->turnOn();
        m_Controller->getTraffic_light().at(3)->getLight()->at(2)->turnOn();

        break;
    case 3:
        m_Controller->getTraffic_light().at(1)->getLight()->at(0)->turnOff();
        m_Controller->getTraffic_light().at(3)->getLight()->at(0)->turnOff();

        m_Controller->getTraffic_light().at(1)->getLight()->at(2)->turnOn();
        m_Controller->getTraffic_light().at(3)->getLight()->at(2)->turnOn();

        break;
    }
    std::thread (&SimulationScene::orangeToRedAndRedToOrange, this, CsNum).detach();

    //orangeToRedAndRedToOrange(CsNum);

/*    TurnOffAllLights();

    if (CsNum==SN_NS_SE_NW)
     {
       m_Controller->getTraffic_light().at(0)->getLight()->at(1)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(1)->turnOn();

       m_Controller->getTraffic_light().at(1)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(3)->turnOn();
     }
    else if (CsNum==SW_NE)
    {
       m_Controller->getTraffic_light().at(0)->getLight()->at(0)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(0)->turnOn();

       m_Controller->getTraffic_light().at(1)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(3)->turnOn();

    }
    else if (CsNum==WE_EW_EN_WS)
    {
       m_Controller->getTraffic_light().at(1)->getLight()->at(1)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(1)->turnOn();

       m_Controller->getTraffic_light().at(0)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(3)->turnOn();

    }
    else if (CsNum==ES_WN)
    {
       m_Controller->getTraffic_light().at(1)->getLight()->at(0)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(0)->turnOn();

       m_Controller->getTraffic_light().at(0)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(3)->turnOn();

    }
*/
             //m_light->at(i)->turnOff();


}

void SimulationScene::TurnOffAllLights()
{
    for(int i=0;i<m_Controller->getTraffic_light().size() ;i++)
    {
        for(int j=0;j<m_Controller->getTraffic_light().at(i)->getLight()->size() ;j++)
        {
            m_Controller->getTraffic_light().at(i)->getLight()->at(j)->turnOff();
        }
    }

}

void SimulationScene::orangeToRedAndRedToOrange(int CsNum)
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    TurnOffAllLights();

    if (CsNum==SN_NS_SE_NW)
     {
       m_Controller->getTraffic_light().at(0)->getLight()->at(2)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(2)->turnOn();

       m_Controller->getTraffic_light().at(1)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(3)->turnOn();
     }
    else if (CsNum==SW_NE)
    {
       m_Controller->getTraffic_light().at(0)->getLight()->at(2)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(2)->turnOn();

       m_Controller->getTraffic_light().at(1)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(3)->turnOn();

    }
    else if (CsNum==WE_EW_EN_WS)
    {
       m_Controller->getTraffic_light().at(1)->getLight()->at(2)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(2)->turnOn();

       m_Controller->getTraffic_light().at(0)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(3)->turnOn();

    }
    else if (CsNum==ES_WN)
    {
       m_Controller->getTraffic_light().at(1)->getLight()->at(2)->turnOn();
       m_Controller->getTraffic_light().at(3)->getLight()->at(2)->turnOn();

       m_Controller->getTraffic_light().at(0)->getLight()->at(3)->turnOn();
       m_Controller->getTraffic_light().at(2)->getLight()->at(3)->turnOn();

    }

    orangeToGreen(CsNum);
}


void SimulationScene::orangeToGreen(int CsNum)
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    if (CsNum==SN_NS_SE_NW)
     {
      m_Controller->getTraffic_light().at(0)->getLight()->at(2)->turnOff();
      m_Controller->getTraffic_light().at(2)->getLight()->at(2)->turnOff();

      m_Controller->getTraffic_light().at(0)->getLight()->at(1)->turnOn();
      m_Controller->getTraffic_light().at(2)->getLight()->at(1)->turnOn();
     }
    else if (CsNum==SW_NE)
    {
      m_Controller->getTraffic_light().at(0)->getLight()->at(2)->turnOff();
      m_Controller->getTraffic_light().at(2)->getLight()->at(2)->turnOff();

      m_Controller->getTraffic_light().at(0)->getLight()->at(0)->turnOn();
      m_Controller->getTraffic_light().at(2)->getLight()->at(0)->turnOn();
    }
    else if (CsNum==WE_EW_EN_WS)
    {
      m_Controller->getTraffic_light().at(1)->getLight()->at(2)->turnOff();
      m_Controller->getTraffic_light().at(3)->getLight()->at(2)->turnOff();

      m_Controller->getTraffic_light().at(1)->getLight()->at(1)->turnOn();
      m_Controller->getTraffic_light().at(3)->getLight()->at(1)->turnOn();
    }
    else if (CsNum==ES_WN)
    {
      m_Controller->getTraffic_light().at(1)->getLight()->at(2)->turnOff();
      m_Controller->getTraffic_light().at(3)->getLight()->at(2)->turnOff();

      m_Controller->getTraffic_light().at(1)->getLight()->at(0)->turnOn();
      m_Controller->getTraffic_light().at(3)->getLight()->at(0)->turnOn();
    }

}

int SimulationScene::getCurrentCsGreen() const
{
    if( m_Controller->getTraffic_light().at(0)->getLight()->at(1)->isOn()==true)
    {
       return 0;
    }
    else if( m_Controller->getTraffic_light().at(0)->getLight()->at(0)->isOn()==true)
    {
       return 1;
    }
    else if( m_Controller->getTraffic_light().at(1)->getLight()->at(1)->isOn()==true)
    {
       return 2;
    }
    else if( m_Controller->getTraffic_light().at(1)->getLight()->at(0)->isOn()==true)
    {
       return 3;
    }
}


