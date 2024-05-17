void PictureLayerImpl::PushPropertiesTo(LayerImpl* base_layer) {
  DoPostCommitInitializationIfNeeded();
  PictureLayerImpl* layer_impl = static_cast<PictureLayerImpl*>(base_layer);
  DCHECK_EQ(layer_impl->is_mask_, is_mask_);

  LayerImpl::PushPropertiesTo(base_layer);

  DCHECK_IMPLIES(twin_layer_, twin_layer_ == layer_impl);
  DCHECK_IMPLIES(twin_layer_, layer_impl->twin_layer_ == this);
  twin_layer_ = layer_impl;
  layer_impl->twin_layer_ = this;

  layer_impl->SetNearestNeighbor(nearest_neighbor_);

  DCHECK_IMPLIES(raster_source_->IsSolidColor(), tilings_->num_tilings() == 0);
  DCHECK_LE(tilings_->num_tilings(),
            layer_tree_impl()->create_low_res_tiling() ? 2u : 1u);

  layer_impl->UpdateRasterSource(raster_source_, &invalidation_,
                                 tilings_.get());
  DCHECK(invalidation_.IsEmpty());

  DCHECK_IMPLIES(raster_source_->IsSolidColor(),
                 layer_impl->tilings_->num_tilings() == 0);

  layer_impl->raster_page_scale_ = raster_page_scale_;
  layer_impl->raster_device_scale_ = raster_device_scale_;
  layer_impl->raster_source_scale_ = raster_source_scale_;
  layer_impl->raster_contents_scale_ = raster_contents_scale_;
  layer_impl->low_res_raster_contents_scale_ = low_res_raster_contents_scale_;

  layer_impl->SanityCheckTilingState();

  layer_impl->needs_post_commit_initialization_ = false;
  needs_post_commit_initialization_ = true;

  needs_push_properties_ = true;
}
