# Query features with Arcade expression

Query features on a map using an Arcade expression.

![](screenshot.png)

## Use case

Arcade is a portable, lightweight, and secure expression language used to create custom content in ArcGIS applications. Like other expression languages, it can perform mathematical calculations, manipulate text, and evaluate logical statements. It also supports multi-statement expressions, variables, and flow control statements. What makes Arcade particularly unique when compared to other expression and scripting languages is its inclusion of feature and geometry data types. This sample uses an Arcade expression to query the number of crimes in a neighborhood in the last 60 days.

## How to use the sample

Click on any neighborhood to see the number of crimes in the last 60 days in a callout.

## How it works

1. Create a `PortalItem` using the URL and ID.
2. Create a `Map` using the portal item.
3. Set the visibility of all the layers to false, except for the layer at position 0.
4. Connect to the `MouseClicked` event on the MapView.
5. Identify the visible layer where it is tapped or clicked on and get the feature.
6. Create the following `ArcadeExpression`:

    "var crimes = FeatureSetByName($map, 'Crime in the last 60 days');\n"
    "return Count(Intersects($feature, crimes));"

7. Create an `ArcadeEvaluator` using the Arcade expression and `ArcadeProfile.FORM_CALCULATION`.
8. Create a map of profile variables with the following key-value pairs. This will be passed to `ArcadeEvaluator::evaluate()` in the next step.

         `{"$feature", identifiedFeature}`
         `{"$map", map}`

9. Call `ArcadeEvaluator::evaluate()` on the Arcade evaluator object and pass the profile variables map.
10. Call `ArcadeEvaluationResult::result()` to get the result from `ArcadeEvaluator::ArcadeEvaluationResult`.
11. Convert the result to a numerical value (integer) and populate the callout with the crime count.

## Relevant API

* ArcadeEvaluationResult
* ArcadeEvaluator
* ArcadeExpression
* ArcadeProfile
* Portal
* PortalItem

## About the data

This sample uses the [Crimes in Police Beats Sample](https://www.arcgis.com/home/item.html?id=14562fced3474190b52d315bc19127f6) ArcGIS Online Web Map which contains 3 layers for police stations, city beats borders, and crimes in the last 60 days as recorded by the Rochester, NY police department.

## Additional information

Visit [Getting Started](https://developers.arcgis.com/arcade/) on the *ArcGIS Developer* website to learn more about Arcade expressions.

## Tags

Arcade evaluator, Arcade expression, identify layers, portal, portal item, query
