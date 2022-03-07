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

#ifndef GRAPHICSMEMORYMANAGEMENTDEMO_H
#define GRAPHICSMEMORYMANAGEMENTDEMO_H

namespace Esri
{
namespace ArcGISRuntime
{
class GraphicsOverlay;
class Map;
class MapQuickView;
class SimpleMarkerSymbol;
}
}

#include <QObject>
#include <Point.h>

class GraphicsMemoryManagementDemo : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(double usedMemory READ usedMemory WRITE setUsedMemory NOTIFY memoryUsageChanged)

public:
  explicit GraphicsMemoryManagementDemo(QObject* parent = nullptr);
  ~GraphicsMemoryManagementDemo() override;

  Q_INVOKABLE void clearGraphics();
  Q_INVOKABLE void badMemoryManagement();
  Q_INVOKABLE void properMemoryManagement();

signals:
  void mapViewChanged();
  void memoryUsageChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  void memoryUsage();
  double usedMemory();
  void setUsedMemory(double usedMemory);

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;

  Esri::ArcGISRuntime::GraphicsOverlay* m_graphicsOverlay = nullptr;

  QScopedPointer<QObject> m_graphicsParent;

  double m_usedMemory;
};

#endif // GRAPHICSMEMORYMANAGEMENTDEMO_H
