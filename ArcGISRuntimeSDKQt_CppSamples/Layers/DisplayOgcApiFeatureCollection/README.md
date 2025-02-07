# Display OGC API Feature Collection

This sample demonstrates how to display an OGC API feature collection and query features while navigating the map view.

![](screenshot.png)

## Use case

When panning the map view, it is necessary to query the OGC API feature table to obtain additional features within the new visible extent.

## How to use the sample

Pan the map and observe how new features are loaded from the OGC API feature service.

## How it works

1. Create an `OgcFeatureCollectionTable` object using a URL to an OGC API feature service and a collection ID.
2. Set the feature table's `FeatureRequestMode` to `ManualCache` so features requested from the server are cached locally.
3. Create a `FeatureLayer` using the feature table and add it to the Map's `operationalLayers`.
4. Every time the map view navigation completes:

    i. Create `QueryParameters`

    ii. Set the parameter's `Geometry` to the current extent of the map view.

    iii. Set the parameter's `SpatialRelationship` to `Intersects`.

    iv. Set the `MaxFeatures` property to 5000 (some services have a low default value for maximum features).

    v. Call `OgcFeatureCollectionTable::PopulateFromService()` using the query parameters from the previous steps.

## Relevant API
* OgcFeatureCollectionTable
* QueryParameters

## Additional information

See the [OGC API website](https://ogcapi.ogc.org/) for more information on the OGC API family of standards.

## Tags

feature, feature layer, feature table, OGC, OGC API, service, table, web
