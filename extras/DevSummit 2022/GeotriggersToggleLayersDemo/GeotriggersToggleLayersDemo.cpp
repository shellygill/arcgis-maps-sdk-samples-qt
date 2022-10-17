// [WriteFile Name=GeotriggersToggleLayersDemo, Category=Analysis]
// [Legal]
// Copyright 2022 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

#include "GeotriggersToggleLayersDemo.h"

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

#include <QDir>

using namespace Esri::ArcGISRuntime;

GeotriggersToggleLayersDemo::GeotriggersToggleLayersDemo(QObject* parent /* = nullptr */):
  QObject(parent)
{
}

GeotriggersToggleLayersDemo::~GeotriggersToggleLayersDemo() = default;

void GeotriggersToggleLayersDemo::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<GeotriggersToggleLayersDemo>("Esri.Samples", 1, 0, "GeotriggersToggleLayersDemoSample");
}

MapQuickView* GeotriggersToggleLayersDemo::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void GeotriggersToggleLayersDemo::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;

  loadMmpk();

  emit mapViewChanged();
}

void GeotriggersToggleLayersDemo::loadMmpk()
{
  MobileMapPackage* mmpk = new MobileMapPackage(QDir::homePath() + "/ArcGIS/Runtime/Data/mmpk/devSummitBerlin2022/BerlinDevSummit_10_13.mmpk", this);

  connect(mmpk, &MobileMapPackage::doneLoading, this, [mmpk, this]()
  {
    m_map = mmpk->maps().at(0);
    m_mapView->setMap(m_map);

    auto viewpoint = Viewpoint(Point(1490859.0920245973, 6893208.26561209, SpatialReference::webMercator()), 2000);
    m_map->setInitialViewpoint(viewpoint);

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

void GeotriggersToggleLayersDemo::runGeotriggers()
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

void GeotriggersToggleLayersDemo::initializeSimulatedLocationDisplay()
{
  m_locationDataSource = new SimulatedLocationDataSource(this);

  SimulationParameters* simulationParameters = new SimulationParameters(QDateTime::currentDateTime(), 10, 0.0, 0.0, this);

  PolylineBuilder polylineBuilder(SpatialReference::webMercator());
  polylineBuilder.addPoint(1490693.7996604848, 6893194.7931713564);
  polylineBuilder.addPoint(1490859.0920245973, 6893208.26561209);
  polylineBuilder.addPoint(1490974.6145278704, 6893211.6140904455);

  m_locationDataSource->setLocationsWithPolyline(polylineBuilder.toPolyline(), simulationParameters);

  m_mapView->locationDisplay()->setDataSource(m_locationDataSource);
  m_mapView->locationDisplay()->setAutoPanMode(LocationDisplayAutoPanMode::Off);
  m_mapView->locationDisplay()->setInitialZoomScale(2000);
}
