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

#include "GraphicsMemoryManagementDemo.h"

#include "Basemap.h"
#include "Map.h"
#include "MapQuickView.h"
#include "Graphic.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleRenderer.h"

// display current RAM
#include<mach/mach.h>

#include <QUrl>

using namespace Esri::ArcGISRuntime;

GraphicsMemoryManagementDemo::GraphicsMemoryManagementDemo(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISTerrain, this))
{
  m_graphicsOverlay = new GraphicsOverlay(this);

  SimpleRenderer* pointRenderer = new SimpleRenderer(new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle,Qt::blue, 5, this), this);
  m_graphicsOverlay->setRenderer(pointRenderer);

  m_graphicsParent.reset(new QObject); // why?????
}

GraphicsMemoryManagementDemo::~GraphicsMemoryManagementDemo()
{
}

MapQuickView* GraphicsMemoryManagementDemo::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void GraphicsMemoryManagementDemo::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
  {
    return;
  }

  m_mapView = mapView;
  m_mapView->setMap(m_map);

  m_mapView->graphicsOverlays()->append(m_graphicsOverlay);

  emit mapViewChanged();
  memoryUsage();

  connect(m_mapView, &MapQuickView::drawStatusChanged, this, [this]()
  {
    memoryUsage();
  });
}

void GraphicsMemoryManagementDemo::clearGraphics()
{
  m_graphicsOverlay->graphics()->clear();
  m_graphicsParent.reset(new QObject);
}

void GraphicsMemoryManagementDemo::badMemoryManagement()
{
  for (int i = 0; i < 100'000; ++i)
  {
    m_graphicsOverlay->graphics()->append(new Graphic(Point(rand()*40,rand()*40), this));
  }
}

void GraphicsMemoryManagementDemo::properMemoryManagement()
{
  for (int i = 0; i < 100'000; ++i)
  {
    m_graphicsOverlay->graphics()->append(new Graphic(Point(rand()*40,rand()*40), m_graphicsParent.get()));
  }
}





// Displays the current resident memory useage of the application

void GraphicsMemoryManagementDemo::memoryUsage()
{
  struct task_basic_info t_info;
  mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
  if (KERN_SUCCESS == task_info(mach_task_self(),
                                TASK_BASIC_INFO, (task_info_t)&t_info,
                                &t_info_count))
  {
    setUsedMemory(t_info.resident_size/1e9);
    emit memoryUsageChanged();
  }
}

double GraphicsMemoryManagementDemo::usedMemory()
{
  return m_usedMemory;
}

void GraphicsMemoryManagementDemo::setUsedMemory(double usedMemory)
{
  m_usedMemory = usedMemory;
}
