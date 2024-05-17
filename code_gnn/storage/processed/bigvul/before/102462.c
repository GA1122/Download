void LayerTreeCoordinator::forceRepaint()
{
    syncDisplayState();

    scheduleLayerFlush();
    flushPendingLayerChanges();
}
