// Copyright 2022 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "BerlinRoute.h"

#include "Basemap.h"
#include "GeometryEngine.h"
#include "Graphic.h"
#include "GraphicsOverlay.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MobileMapPackage.h"
#include "PolylineBuilder.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleRenderer.h"
#include "SpatialReference.h"

#include <QUrl>
#include <QDir>
#include <QMouseEvent>
#include <QStandardPaths>

using namespace Esri::ArcGISRuntime;

// helper method to get cross platform data path
namespace
{
QString dataPath_()
{
  QString dataPath;

#ifdef Q_OS_ANDROID
  dataPath = "/sdcard";
#elif defined Q_OS_IOS
  dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
  dataPath = QDir::homePath();
#endif

  dataPath += "/ArcGIS/Runtime/Data/mmpk/devSummitBerlin2022/";

  return dataPath;
}

QString mmpkName_()
{
  //return QString{"geotriggers.mmpk"};
  return QString{"BerlinDevSummitFloors_2022.mmpk"};
}

QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}

BerlinRoute::BerlinRoute(QObject* parent /* = nullptr */):
  QObject(parent),
  m_graphicsOverlay(new GraphicsOverlay(this)),
  m_polylineBuilder(new PolylineBuilder(SpatialReference(3857), this)),
  m_logFile(new QFile(logFilePath.append("/BerlinRoute" + QString::number(QDateTime::currentSecsSinceEpoch()) + ".csv"), this))
{
  if (!m_logFile->open(QFile::WriteOnly))
  {
    qDebug() << "LocationLogger: Failed to open file";
    return;
  }

  // add column headers to file
  m_logFile->write("x,y,wkid,horizontalAccuracy,velocity,course,timestamp\n");

  // assign a renderer to the graphics overlay
  SimpleMarkerSymbol* simpleMarkerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, Qt::red, 10, this);
  SimpleRenderer* simpleRenderer = new SimpleRenderer(simpleMarkerSymbol, this);
  m_graphicsOverlay->setRenderer(simpleRenderer);

  auto dataPath = dataPath_();
  auto mmpkPath = dataPath + mmpkName_();

  if (!QFileInfo::exists(mmpkPath))
  {
    qDebug() << QString{"%1 not found!"}.arg(dataPath);
    return;
  }

  auto mmpk = new MobileMapPackage(mmpkPath);

  connect(mmpk, &MobileMapPackage::doneLoading, this, [this, mmpk](const Error& /*loadError*/)
  {
    if (mmpk->loadStatus() != LoadStatus::Loaded)
    {
      return;
    }

    auto maps = mmpk->maps();
    if (maps.length() < int{1})
    {
      qDebug() << "No maps in mmpk";
      return;
    }

    m_map = maps.at(0);

    connect(m_map, &Map::doneLoading, this, [this](const Error& loadError)
    {
      if (!loadError.isEmpty())
      {
        return;
      }

      if (m_map->loadStatus() != LoadStatus::Loaded)
      {
        return;
      }

      if (m_mapView)
      {
        m_mapView->setMap(m_map);
      }
    });

    m_map->load();
  });

  mmpk->load();
}

BerlinRoute::~BerlinRoute()
{
  if (m_logFile)
    m_logFile->close();
}

MapQuickView* BerlinRoute::mapView() const
{
  return m_mapView;
}

//Geometry createPolyline()
//{
//  // Create a polyline builder
//  const SpatialReference spatialRef(4326);
//  PolylineBuilder polylineBuilder(spatialRef);
//}

// Set the view (created in QML)
void BerlinRoute::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
  {
    return;
  }

  m_mapView = mapView;
  if (m_map)
  {
    m_mapView->setMap(m_map);
  }

  m_mapView->graphicsOverlays()->append(m_graphicsOverlay);

//  auto point = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());
//  auto pointWgs84 = Point(GeometryEngine::project(point, SpatialReference(4326)));
//  m_polylineBuilder->addPoint(pointWgs84);
  Graphic* graphic = new Graphic(Point(12.39282, 52.5122), this);
  m_graphicsOverlay->graphics()->append(graphic);

  connect(m_mapView, &MapQuickView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
  {
    auto point = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());
    auto pointWgs84 = Point(GeometryEngine::project(point, SpatialReference(4326)));
    m_polylineBuilder->addPoint(pointWgs84);
    qDebug() << pointWgs84.toJson();
    Graphic* graphic = new Graphic(pointWgs84, this);
    m_graphicsOverlay->graphics()->append(graphic);

    // write to file
    QStringList parts
    {
      QString::number(pointWgs84.x(), 'g', 18),
      QString::number(pointWgs84.y(), 'g', 18),
      QString::number(pointWgs84.spatialReference().wkid()),
      QString::number(4),
      QString::number(0),
      QString::number(0),
      QString::number(1637260000000)
    };

    QString data = parts.join(",").append("\n");

    if (m_logFile->write(data.toLatin1()) == -1)
    {
       qDebug() << "LocationLogger: Error writing to file";
    }

    m_logFile->flush();
  });

  connect(m_mapView, &MapQuickView::mouseDoubleClicked, this, [this](QMouseEvent& /*mouseEvent*/){
    qDebug() << m_polylineBuilder->toPolyline().toJson();
  });

  emit mapViewChanged();
}
