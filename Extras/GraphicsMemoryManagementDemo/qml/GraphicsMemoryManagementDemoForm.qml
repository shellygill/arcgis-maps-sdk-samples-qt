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

import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.GraphicsMemoryManagementDemo 1.0

Item {

    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        id: view
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    GraphicsMemoryManagementDemo {
        id: gmmDemo
        mapView: view
    }

    Rectangle {
        color: "white"
        border.color: "black"
        x: 5
        y: 5
        width: col.width + 15
        height: col.height + 15

        Column {
            id: col
            anchors.centerIn: parent
            spacing: 10

            Button {
                id: button
                width: parent.width
                Text {
                    text: "Bad memory management"
                    anchors.centerIn: parent
                }
                onClicked: gmmDemo.badMemoryManagement();
            }

            Button {
                width: parent.width
                Text {
                    text: "Proper memory management"
                    anchors.centerIn: parent
                }
                onClicked: gmmDemo.properMemoryManagement();
            }

            Button {
                width: parent.width
                Text {
                    text: "Clear graphics"
                    anchors.centerIn: parent
                }
                onClicked: gmmDemo.clearGraphics();
            }

            Text {
                text: "Resident memory usage: " + (gmmDemo.usedMemory).toFixed(3) + " GB";
            }
        }
    }
}
