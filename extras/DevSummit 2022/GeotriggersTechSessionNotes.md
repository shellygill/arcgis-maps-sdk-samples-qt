## GraphicFenceParameters demo

- Let's take a look at a Qt app that demonstrates a simple use of geotriggers.
- Here, I've got a map and I've placed a point graphic at the center of the screen. The geotrigger is monitoring the area covered by this dotted line, which consists of the point graphic buffered by a radius. The geotrigger sends a notification when the fence is entered or exited, and we're using the type of notification to add a green or red graphic to the map depending on whether it is an enter or exit notification.
- So in this example, we're monitoring a fence that is a point buffered by a radius, and executing some action when there is a notification.
- We can also monitor a polygon. Let's take a look at how that would look.
- Here, I've added a light blue polygon to the map. That polygon is the fence. As before, when the fence is entered we add a green polygon to the point of entry, and when the fence is exited we add a red polygon.

## FeatureFenceParameters demo

- This time, let's take a look at using features as the fences we're monitoring. Here I've got a mobile map package that contains a map and a feature layer that shows some parcels. I'm using these parcel features as the fences I'm monitoring.
- So we can see that as the location passes through this neighborhood, all the parcels we pass through are highlighted. This ends up being a lot of features being highlighted.
- Let's say we're only interested in getting a notification when parcels over a certain acreage are entered. We can apply a whereClause to the feature layer to only create fences for the features that have a recorded acreage of more that 0.18 acres.
- So now when we start the geotrigger, we see fewer fences are highlighted.

## ReadGeotriggers - using horizontal accuracy

- Here, I've loaded a mobile map package that defines some geotriggers around the Palm Springs area.
- I'm using a simulated location data source and clicking on the screen to simulate a location update.
- Let's say the user has approached this fence. You see the fence get highlighted to indicate an entry. Maybe there's a bit of bounce, so the location moves slightly out of the fence. You get an exit notification. And again move just slightly, you get an enter. In this case, we're getting spammed with a bunch of notifications that we don't need. So let's use the horizontal accuracy of the location updates to reduce the number of notifications we get.
- Let's use the horizontal accuracy and say we want notifications when the horizontal accuracy intersects the fence. So here, even though the location is bouncing in and out of the fence, we only get the one enter notification because the accuracy bubble continues to intersect the fence.
- And as Luke said, you can consider your particular needs to determine how to use the horizontal accuracy to filter notifications.

## Geotriggers.cpp

- Let's take a look at how to get to the feature that triggered the notification.
- From the FenceGeotriggerNotificaionInfo, we can get the feature that triggered the notification.
- Now that we have the feature, we can do whatever we want with it.
- In this case, we're using the `description` attribute to provide the text in the popup, and the attachment provides the image.
