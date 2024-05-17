void LayerTreeHostQt::didInstallPageOverlay()
{
    createPageOverlayLayer();
    scheduleLayerFlush();
}
