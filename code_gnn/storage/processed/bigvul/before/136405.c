  SynthesizedClip() : layer_(cc::PictureLayer::Create(this)) {
    mask_isolation_id_ =
        CompositorElementIdFromUniqueObjectId(NewUniqueObjectId());
    mask_effect_id_ =
        CompositorElementIdFromUniqueObjectId(NewUniqueObjectId());
    layer_->SetIsDrawable(true);
  }
