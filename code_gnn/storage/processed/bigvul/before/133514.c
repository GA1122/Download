scoped_ptr<EdgeEffectBase> OverscrollControllerAndroid::CreateEdgeEffect() {
  return CreateGlowEdgeEffect(&compositor_->GetResourceManager(), dpi_scale_);
}
