// [WriteFile Name=ChooseCameraController, Category=Scenes]
// [Legal]
// Copyright 2019 Esri.

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

#include "ChooseCameraController.h"

#include "ArcGISTiledElevationSource.h"
#include "GlobeCameraController.h"
#include "ModelSceneSymbol.h"
#include "OrbitLocationCameraController.h"
#include "OrbitGeoElementCameraController.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "MapTypes.h"
#include "TaskWatcher.h"
#include "GraphicsOverlayListModel.h"
#include "GraphicListModel.h"
#include "Camera.h"
#include "Surface.h"
#include "ElevationSourceListModel.h"
#include "SceneViewTypes.h"
#include "LayerSceneProperties.h"
#include "SpatialReference.h"
#include "GraphicsOverlay.h"
#include "Point.h"
#include "Graphic.h"

#include <QtCore/qglobal.h>
#include <QStandardPaths>

namespace
{
  const double longitude = -109.929589;
  const double latitude = 38.43500;
  const double distance = 5000;

  QString defaultDataPath()
  {
    QString dataPath;

  #ifdef Q_OS_IOS
    dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  #else
    dataPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  #endif

    return dataPath;
  }
}

using namespace Esri::ArcGISRuntime;

ChooseCameraController::ChooseCameraController(QObject* parent /* = nullptr */):
  QObject(parent),
  m_orbitGlobe(new GlobeCameraController(this)),
  m_orbitLocation(new OrbitLocationCameraController(Point(longitude, latitude, 0.0, SpatialReference::wgs84()), 100, this)),
  m_scene(new Scene(BasemapStyle::ArcGISImageryStandard, this))
{
  // create a new elevation source from Terrain3D REST service
  ArcGISTiledElevationSource* elevationSource = new ArcGISTiledElevationSource(
        QUrl("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"), this);

  // add the elevation source to the scene to display elevation
  m_scene->baseSurface()->elevationSources()->append(elevationSource);

  m_orbitLocation->setCameraDistance(distance);

  // get the data path
  QUrl dataUrl(defaultDataPath() + "/ArcGIS/Runtime/Data/3D/Bristol/Collada/Bristol.dae");
  ModelSceneSymbol* planeModel = new ModelSceneSymbol(dataUrl, 10.0f, this);

  Esri::ArcGISRuntime::Graphic* graphic = new Graphic(Point(longitude, latitude, distance / 2), planeModel, this);

  m_overlay = new GraphicsOverlay(this);

  m_overlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));
  m_overlay->graphics()->append(graphic);
  m_orbitGeoElement = new OrbitGeoElementCameraController(graphic, distance / 2, this);
}

ChooseCameraController::~ChooseCameraController() = default;

void ChooseCameraController::setGlobeController()
{
  if(m_sceneView != nullptr)
  {
    m_sceneView->setCameraController(m_orbitGlobe);
  }
}

void ChooseCameraController::setOrbitLocationController()
{
  if(m_sceneView != nullptr)
  {
    m_sceneView->setCameraController(m_orbitLocation);
  }
}

void ChooseCameraController::setOrbitGeoElementController()
{
  if(m_sceneView != nullptr)
  {
    m_sceneView->setCameraController(m_orbitGeoElement);
  }
}

void ChooseCameraController::init()
{
  // Register classes for QML
  qmlRegisterType<SceneQuickView>("Esri.Samples", 1, 0, "SceneView");
  qmlRegisterType<ChooseCameraController>("Esri.Samples", 1, 0, "ChooseCameraControllerSample");
}

SceneQuickView* ChooseCameraController::sceneView() const
{
  return m_sceneView;
}

// Set the view (created in QML)
void ChooseCameraController::setSceneView(SceneQuickView* sceneView)
{
  if (!sceneView || sceneView == m_sceneView)
  {
    return;
  }

  m_sceneView = sceneView;
  m_sceneView->setViewpointCamera(Camera(latitude, longitude, distance, 0.0, 0.0, 0.0));
  m_sceneView->setArcGISScene(m_scene);
  m_sceneView->graphicsOverlays()->append(m_overlay);

  emit sceneViewChanged();
}

