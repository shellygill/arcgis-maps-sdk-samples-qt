<!-- markdownlint-disable -->

## Demo 1: After "Geotrigger Condition: FenceRuleType & FenceParameters"

- Build up the app throughout the session
- `GeotriggersDemo.cpp`
  - create a polygon graphic, add to map, use it as the fence
  - then show using a point graphic

  - add button for toggle polygon?

- Demo with parcels - `GeotriggersParcelDemo`
  - To show using feature fences
  - Show adding whereClause to fenceParams
  
  - add whereClause button

This gives an overview of the API.

## "Geotrigger Input: LocationDataSource"

- Use Arcade to filter feed
- `ReadGeotriggers` app
  - Use horizontalAccuracy to reduce "spam" notifications

## "Geotrigger Actions: FeedGeotriggerNotificationInfo"

- More than just notification messages - highlight features, etc
- `GeotriggersToggleLayersDemo`
  - Enter the Palm Springs Convention center, show floor layout when in fence, show basemap when out
- `Geotriggers sample`

- Some demo about going through code, how to create a geotrigger
- Read from a map
  - get read geotriggers app
  - should fieldmaps app of geotriggers authoring

---

Optimizing data for working offline

## Basemap by reference

- Look at webmap in AGOL to show the map won't download basemap
- then look in app, and show what stuff is downloaded
  - `Offline basemap by reference`

**Sample exists**

## Polygon areas

- Look at preplanned area with 2 areas
- `Offline map with polygon area`
  - DownloadPreplannedOfflineMapJob, just need to download the right area

**No sample for this, get the right data**

**Sample exists**

## API options - including online only layers / overrides

Generate offline map:

- leave basemap online
- remove trees feature layer
- only take certain building features offline (using query)

Notes: 
- when you click the button, say what it's doing
- only go into the code of one override

**Sample for** `GenerateOfflineMapParametersOverrides`, none for online-only layers

## Mobile geodatabase

- Show mobile geodatabase in pro - new > mobile geodatabase -> then import features etc
- Add data to Runtime App -> the new data will show up in Pro

**Sample for showing mobile geodatabase** - removes features. Can update to add features as well?



--- 

Notes on geotriggers: 
- Geotrigger doesn't need to have a name

- Show code first
- Add play button

- Add floor picker