// [WriteFile Name=BlendRasterLayer, Category=Layers]
// [Legal]
// Copyright 2017 Esri.

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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Esri.Samples

BlendRasterLayerSample {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    property bool editingRenderer: false

    SlopeTypeModel {
        id: slopeTypeModel
    }

    ColorRampModel {
        id: colorRampModel
    }

    // add a mapView component
    MapView {
        anchors.fill: parent
        objectName: "mapView"

        Component.onCompleted: {
            // Set the focus on MapView to initially enable keyboard navigation
            forceActiveFocus();
        }
    }

    Rectangle {
        visible: editButton.visible
        anchors.centerIn: editButton
        radius: 8
        height: editButton.height + (16)
        width: editButton.width + (16)
        color: "lightgrey"
        border.color: "darkgrey"
        border.width: 2
        opacity: 0.75
    }

    Button {
        id: editButton
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            margins: 32
        }
        visible: rendererBox.width === 0
        text: "Edit Renderer"
        onClicked: editingRenderer = true;
    }

    Rectangle {
        id: rendererBox
        clip: true
        anchors {
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        color: "white"
        opacity: 0.75
        width: editingRenderer ? parent.width : 0

        GridLayout {
            anchors {
                centerIn: parent
                margins: 24
            }

            columns: 2

            Text {
                text: "altitude"
            }

            SpinBox {
                id: altSpinBox
                from: 0
                to: 90
                editable: true
                textFromValue: function(value) {
                    return value.toFixed(0) + "\u00B0";
                }
                valueFromText: function(text) {
                    return parseInt(text);
                }
            }

            Text {
                text: "azimuth"
            }

            SpinBox {
                id: azimuthSpinBox
                from: 0
                to: 360
                editable: true
                textFromValue: function(value) {
                    return value.toFixed(0) + "\u00B0";
                }
                valueFromText: function(text) {
                    return parseInt(text);
                }
            }

            Text {
                text: "slope type"
            }

            ComboBox {
                id: slopeCombo
                property int modelWidth: 0
                Layout.minimumWidth: modelWidth + leftPadding + rightPadding
                Layout.fillWidth: true
                textRole: "name"
                model: slopeTypeModel
                Component.onCompleted : {
                    for (let i = 0; i < model.count; ++i) {
                        metrics.text = model.get(i).name;
                        modelWidth = Math.max(modelWidth, metrics.width);
                    }
                }
                TextMetrics {
                    id: metrics
                    font: slopeCombo.font
                }
            }

            Text {
                text: "color ramp"
            }

            ComboBox {
                id: colorCombo
                property int modelWidth: 0
                Layout.minimumWidth: modelWidth + leftPadding + rightPadding
                Layout.fillWidth: true
                textRole: "name"
                model: colorRampModel
                Component.onCompleted : {
                    for (let i = 0; i < model.count; ++i) {
                        metrics2.text = model.get(i).name;
                        modelWidth = Math.max(modelWidth, metrics2.width);
                    }
                }
                TextMetrics {
                    id: metrics2
                    font: colorCombo.font
                }
            }

            Button {
                text: "Render"
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    editingRenderer = false;
                    applyRenderSettings(altSpinBox.value,
                                        azimuthSpinBox.value,
                                        slopeTypeModel.get(slopeCombo.currentIndex).value,
                                        colorRampModel.get(colorCombo.currentIndex).value);
                }
            }
        }

        Behavior on width { PropertyAnimation { duration: 500 } }
    }
}
