void PictureLayerImpl::AsValueInto(base::debug::TracedValue* state) const {
  const_cast<PictureLayerImpl*>(this)->DoPostCommitInitializationIfNeeded();
  LayerImpl::AsValueInto(state);
  state->SetDouble("ideal_contents_scale", ideal_contents_scale_);
  state->SetDouble("geometry_contents_scale", MaximumTilingContentsScale());
  state->BeginArray("tilings");
  tilings_->AsValueInto(state);
  state->EndArray();

  state->BeginArray("tile_priority_rect");
  MathUtil::AddToTracedValue(GetViewportForTilePriorityInContentSpace(), state);
  state->EndArray();

  state->BeginArray("visible_rect");
  MathUtil::AddToTracedValue(visible_content_rect(), state);
  state->EndArray();

  state->BeginArray("pictures");
  raster_source_->AsValueInto(state);
  state->EndArray();

  state->BeginArray("invalidation");
  invalidation_.AsValueInto(state);
  state->EndArray();

  state->BeginArray("coverage_tiles");
  for (PictureLayerTilingSet::CoverageIterator iter(
           tilings_.get(), 1.f, gfx::Rect(raster_source_->GetSize()),
           ideal_contents_scale_);
       iter; ++iter) {
    state->BeginDictionary();

    state->BeginArray("geometry_rect");
    MathUtil::AddToTracedValue(iter.geometry_rect(), state);
    state->EndArray();

    if (*iter)
      TracedValue::SetIDRef(*iter, state, "tile");

    state->EndDictionary();
  }
  state->EndArray();
}
