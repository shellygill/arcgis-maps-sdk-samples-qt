// [WriteFile Name=FeatureLayerExtrusion, Category=Scenes]
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
import Esri.Samples

FeatureLayerExtrusionSample {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    SceneView {
        id: sceneView
        objectName: "sceneView"
        anchors.fill: parent

        Component.onCompleted: {
            // Set the focus on SceneView to initially enable keyboard navigation
            forceActiveFocus();
        }

        // combo box to update the extrusion
        ComboBox {
            id: popCombo
            anchors {
                top: parent.top
                left: parent.left
                margins: 10
            }

            property int modelWidth: 0
            width: modelWidth + leftPadding + rightPadding

            model: ["TOTAL POPULATION", "POPULATION DENSITY"]

            onCurrentTextChanged: {
                if (currentText === "TOTAL POPULATION")
                    totalPopulation();
                else
                    popDensity();
            }

            Component.onCompleted : {
                for (let i = 0; i < model.length; ++i) {
                    metrics.text = model[i];
                    modelWidth = Math.max(modelWidth, metrics.width);
                }
            }
            TextMetrics {
                id: metrics
                font: popCombo.font
            }
        }
    }
}
