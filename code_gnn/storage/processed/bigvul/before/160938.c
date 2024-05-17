float ChromeClientImpl::ClampPageScaleFactorToLimits(float scale) const {
  return web_view_->ClampPageScaleFactorToLimits(scale);
}
