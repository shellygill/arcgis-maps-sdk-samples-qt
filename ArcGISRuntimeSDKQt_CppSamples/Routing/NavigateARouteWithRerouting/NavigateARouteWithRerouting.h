// [WriteFile Name=NavigateARouteWithRerouting, Category=Routing]
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

#ifndef NAVIGATEAROUTEWITHREROUTING_H
#define NAVIGATEAROUTEWITHREROUTING_H

#include "Route.h"
#include "RouteResult.h"
#include "RouteParameters.h"
#include "DirectionManeuver.h"

// NOTE: As of Qt 6.2, QTextToSpeech is not supported. Instances of this class have been commented out for compatibility, but remain for reference
// class QTextToSpeech;

namespace Esri
{
namespace ArcGISRuntime
{
class Graphic;
class GraphicsOverlay;
class Map;
class MapQuickView;
class RouteParameters;
class RouteTask;
class RouteTracker;
class SimulatedLocationDataSource;
}
}

#include <QObject>

Q_MOC_INCLUDE("MapQuickView.h")

class NavigateARouteWithRerouting : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(bool navigationEnabled READ navigationEnabled NOTIFY navigationEnabledChanged)
  Q_PROPERTY(bool recenterEnabled READ recenterEnabled NOTIFY recenterEnabledChanged)
  Q_PROPERTY(QString textString READ textString NOTIFY textStringChanged)

public:
  explicit NavigateARouteWithRerouting(QObject* parent = nullptr);
  ~NavigateARouteWithRerouting();

  static void init();
  Q_INVOKABLE void startNavigation();
  Q_INVOKABLE void recenterMap();

signals:
  void mapViewChanged();
  void navigationEnabledChanged();
  void recenterEnabledChanged();
  void textStringChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  bool navigationEnabled() const;
  bool recenterEnabled() const;
  QString textString() const;
  void connectRouteTaskSignals();
  void connectRouteTrackerSignals();

  Esri::ArcGISRuntime::Graphic* m_routeAheadGraphic = nullptr;
  Esri::ArcGISRuntime::Graphic* m_routeTraveledGraphic = nullptr;
  Esri::ArcGISRuntime::GraphicsOverlay* m_routeOverlay = nullptr;
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::RouteTask* m_routeTask = nullptr;
  Esri::ArcGISRuntime::Route m_route;
  Esri::ArcGISRuntime::RouteResult m_routeResult;
  Esri::ArcGISRuntime::RouteTracker* m_routeTracker = nullptr;
  Esri::ArcGISRuntime::SimulatedLocationDataSource* m_simulatedLocationDataSource = nullptr;
  bool m_navigationEnabled = false;
  bool m_recenterEnabled = false;
  QString m_textString = "";
  Esri::ArcGISRuntime::RouteParameters m_routeParameters;
  QList<Esri::ArcGISRuntime::DirectionManeuver> m_directionManeuvers;
  // QTextToSpeech* m_speaker = nullptr;
};

#endif // NAVIGATEAROUTEWITHREROUTING_H
