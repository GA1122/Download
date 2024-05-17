float PictureLayerImpl::MaximumTilingContentsScale() const {
  float max_contents_scale = tilings_->GetMaximumContentsScale();
  return std::max(max_contents_scale, MinimumContentsScale());
}
