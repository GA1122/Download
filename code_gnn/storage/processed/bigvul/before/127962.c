void BrowserViewRenderer::SetOffscreenPreRaster(bool enable) {
  if (offscreen_pre_raster_ != enable && compositor_)
    UpdateMemoryPolicy();

  offscreen_pre_raster_ = enable;
}
