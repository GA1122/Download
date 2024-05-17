PaintArtifactCompositor::PaintArtifactCompositor(
    base::RepeatingCallback<void(const gfx::ScrollOffset&,
                                 const cc::ElementId&)> scroll_callback)
    : scroll_callback_(std::move(scroll_callback)),
      tracks_raster_invalidations_(false) {
  if (!RuntimeEnabledFeatures::SlimmingPaintV2Enabled() &&
      !RuntimeEnabledFeatures::BlinkGenPropertyTreesEnabled())
    return;
  root_layer_ = cc::Layer::Create();
}
