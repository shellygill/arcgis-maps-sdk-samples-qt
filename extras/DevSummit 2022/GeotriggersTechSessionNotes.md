
## ReadGeotriggers - using horizontal accuracy

- Here, I've loaded a mobile map package that defines some geotriggers.
- I'm using a simulated location data source and clicking on the screen to simulate a location update.
- Let's say the user has approached this fence. You see the fence get highlighted to indicate an entry. Maybe there's a bit of bounce, so the location moves slightly out of the fence. You get an exit notification. And again move just slightly, you get an enter. In this case, we're getting spammed with a bunch of notifications that we don't need. So let's use the horizontal accuracy of the location updates to reduce the number of notifications we get.
- Let's use the horizontal accuracy and say we want notifications when the horizontal accuracy intersects the fence. So here, even though the location is bouncing in and out of the fence, we only get the one enter notification because the accuracy bubble continues to intersect the fence.
- And as Luke said, you can consider your particular needs to determine how to use the horizontal accuracy to filter notifications.

- Use a different map

## Geotriggers.cpp

- Let's take a look at how to get to the feature that triggered the notification.
- From the FenceGeotriggerNotificaionInfo, we can get the feature that triggered the notification.
- Now that we have the feature, we can do whatever we want with it.
- In this case, we're using the `description` attribute to provide the text in the popup, and the attachment provides the image.
