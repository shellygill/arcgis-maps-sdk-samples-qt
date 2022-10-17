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

#ifndef GEOTRIGGERSINDOORSDEMO_H
#define GEOTRIGGERSINDOORSDEMO_H

namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapQuickView;
class SimulatedLocationDataSource;
class LocationGeotriggerFeed;
class FeatureLayer;
class FeatureTable;
class FloorManager;
}
}

#include <QObject>

class GeotriggersIndoorsDemo : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)

public:
  explicit GeotriggersIndoorsDemo(QObject* parent = nullptr);
  ~GeotriggersIndoorsDemo() override;

signals:
  void mapViewChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  void loadMmpk();
  void initializeSimulatedLocationDisplay();
  void runGeotriggers();

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::SimulatedLocationDataSource* m_locationDataSource = nullptr;

  Esri::ArcGISRuntime::LocationGeotriggerFeed* m_geotriggerFeed = nullptr;
  Esri::ArcGISRuntime::FeatureLayer* m_indoorLayers = nullptr;
  Esri::ArcGISRuntime::FeatureTable* m_buildingFootprintTable = nullptr;
  Esri::ArcGISRuntime::FeatureLayer* m_layerToToggle = nullptr;
  Esri::ArcGISRuntime::FloorManager* m_floorManager = nullptr;
};

#endif // GEOTRIGGERSINDOORSDEMO_H
