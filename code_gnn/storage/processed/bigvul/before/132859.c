bool PictureLayerImpl::ShouldAdjustRasterScaleDuringScaleAnimations() const {
  return layer_tree_impl()->use_gpu_rasterization();
}
