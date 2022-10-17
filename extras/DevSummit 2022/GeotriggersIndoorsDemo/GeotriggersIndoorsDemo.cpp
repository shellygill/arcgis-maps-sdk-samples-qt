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

#include "GeotriggersIndoorsDemo.h"

#include "Map.h"
#include "MapQuickView.h"
#include "MobileMapPackage.h"

#include "ArcadeExpression.h"
#include "SimulatedLocationDataSource.h"
#include "SimulationParameters.h"
#include "PolylineBuilder.h"
#include "LocationGeotriggerFeed.h"
#include "FenceGeotrigger.h"
#include "FeatureFenceParameters.h"
#include "GeotriggerMonitor.h"
#include "FenceGeotriggerNotificationInfo.h"
#include "GeotriggerNotificationInfo.h"
#include "GeotriggersTypes.h"
#include "FeatureTable.h"
#include "ServiceFeatureTable.h"
#include "FeatureLayer.h"
#include "FloorManager.h"
#include "FloorLevel.h"
#include "LocationSourcePropertiesKeys.h"

#include <QDir>

using namespace Esri::ArcGISRuntime;

GeotriggersIndoorsDemo::GeotriggersIndoorsDemo(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISStreets, this))
{
}

GeotriggersIndoorsDemo::~GeotriggersIndoorsDemo()
{
}

MapQuickView* GeotriggersIndoorsDemo::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void GeotriggersIndoorsDemo::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;

  loadMmpk();

  emit mapViewChanged();
}

void GeotriggersIndoorsDemo::loadMmpk()
{
  MobileMapPackage* mmpk = new MobileMapPackage(QDir::homePath() + "/ArcGIS/Runtime/Data/mmpk/devSummitBerlin2022/BerlinDevSummit_10_13.mmpk", this);

  connect(mmpk, &MobileMapPackage::doneLoading, this, [mmpk, this]()
  {
    m_map = mmpk->maps().at(0);
    m_mapView->setMap(m_map);

    auto viewpoint = Viewpoint(Point(1490859.0920245973, 6893208.26561209, SpatialReference::webMercator()), 2000);
    m_map->setInitialViewpoint(viewpoint);

    connect(m_map, &Map::doneLoading, this, [this](Error loadError)
    {
      if (m_map->loadStatus() != LoadStatus::Loaded)
      {
        return;
      }

      // Load floor manager
      m_floorManager = m_map->floorManager();

      if (m_floorManager == nullptr)
      {
        qDebug() << "null floormanager";
      }
      else
      {
        qDebug("got floormanager");
      }

      connect(m_floorManager, &FloorManager::errorOccurred, this, [](Error err)
      {
        if (!err.isEmpty())
        {
          qDebug() << err.message() << err.additionalMessage();
        }
      });

      connect(m_floorManager, &FloorManager::doneLoading, this, [this](const Error& /*loadError*/)
      {

        // Initially don't show any levels some changes
        if (m_floorManager && m_floorManager->loadStatus() == LoadStatus::Loaded)
        {
          for (FloorLevel* level : m_floorManager->levels())
          {
            level->setVisible(false);
          }
        }
      });

      m_floorManager->load();
    });

    m_map->load();

    for (auto layer : *m_map->operationalLayers())
    {
      qDebug() << layer->name();
    }

    m_indoorLayers = dynamic_cast<FeatureLayer*>(m_map->operationalLayers()->at(1));
    m_indoorLayers->setVisible(false);

    m_layerToToggle = dynamic_cast<FeatureLayer*>(m_map->operationalLayers()->at(3));
    m_layerToToggle->setVisible(false);

    m_buildingFootprintTable = m_indoorLayers->featureTable();

    initializeSimulatedLocationDisplay();
    runGeotriggers();
  });

  mmpk->load();
}


void GeotriggersIndoorsDemo::runGeotriggers()
{
  // Use the user's location as input data
  m_geotriggerFeed = new LocationGeotriggerFeed(m_locationDataSource, this);

  // Monitor the convention center footprint polygon
  FeatureFenceParameters* featureFenceParameters = new FeatureFenceParameters(m_buildingFootprintTable, this);

  // Create a fence geotrigger
  FenceGeotrigger* fenceGeotrigger = new FenceGeotrigger(m_geotriggerFeed, FenceRuleType::EnterOrExit, featureFenceParameters, this);

  // Monitor the fence geotrigger
  GeotriggerMonitor* geotriggerMonitor = new GeotriggerMonitor(fenceGeotrigger, this);

  connect(geotriggerMonitor, &GeotriggerMonitor::geotriggerNotification, this, [this](GeotriggerNotificationInfo* geotriggerNotificationInfo)
  {
    FenceGeotriggerNotificationInfo* fenceGeotriggerNotificationInfo = static_cast<FenceGeotriggerNotificationInfo*>(geotriggerNotificationInfo);

    // When we enter the feature fence...
    if (fenceGeotriggerNotificationInfo->fenceNotificationType() == FenceNotificationType::Entered)
    {
      // show the indoor layers and hide the basemap
      m_layerToToggle->setVisible(true);
      m_map->operationalLayers()->at(0)->setVisible(false);
    }
    // otherwise...
    else
    {
      // hide the indoor layers and show the basemap
      m_layerToToggle->setVisible(false);
      m_map->operationalLayers()->at(0)->setVisible(true);
    }
  });

  m_mapView->locationDisplay()->start();
  m_locationDataSource->start();
  geotriggerMonitor->start();
}

void GeotriggersIndoorsDemo::initializeSimulatedLocationDisplay()
{
  m_locationDataSource = new SimulatedLocationDataSource(this);

  auto startTime = QDateTime::currentDateTime();

  auto createLocWithFloor = [&startTime](Point position, int floor)
  {
    startTime = startTime.addSecs(1);

    QVariantMap additionalSourceProperties;
    additionalSourceProperties.insert(LocationSourcePropertiesKeys::floor(), floor);

    return Location(startTime, position, 0.0, 0.0, 0.0, 123, false, additionalSourceProperties);
  };

  QList<Location> locations = {
    createLocWithFloor(Point(1490693.7996604848, 6893194.7931713564, SpatialReference::webMercator()), 0),
    createLocWithFloor(Point(1490859.0920245973, 6893208.26561209, SpatialReference::webMercator()), 0),
    createLocWithFloor(Point(1490859.0920245973, 6893208.26561209, SpatialReference::webMercator()), 1),
    createLocWithFloor(Point(1490974.6145278704, 6893211.6140904455, SpatialReference::webMercator()), 0)
  };

  m_locationDataSource->setLocations(locations);

  m_mapView->locationDisplay()->setDataSource(m_locationDataSource);
  m_mapView->locationDisplay()->setAutoPanMode(LocationDisplayAutoPanMode::Off);
  m_mapView->locationDisplay()->setInitialZoomScale(2000);
}
