void ConversionContext::EndEffect() {
  const auto& previous_state = state_stack_.back();
  DCHECK_EQ(previous_state.type, StateEntry::kEffect);
  DCHECK_EQ(current_effect_->Parent(), previous_state.effect);
  DCHECK_EQ(current_clip_, previous_state.clip);

  DCHECK(effect_bounds_stack_.size());
  const auto& bounds_info = effect_bounds_stack_.back();
  FloatRect bounds = bounds_info.bounds;
  if (!bounds.IsEmpty()) {
    if (current_effect_->Filter().IsEmpty()) {
      cc_list_.UpdateSaveLayerBounds(bounds_info.save_layer_id, bounds);
    } else {
      auto save_layer_bounds = bounds;
      save_layer_bounds.MoveBy(-current_effect_->PaintOffset());
      cc_list_.UpdateSaveLayerBounds(bounds_info.save_layer_id,
                                     save_layer_bounds);
      bounds = current_effect_->MapRect(bounds);
    }
  }

  effect_bounds_stack_.pop_back();
  EndTransform();
  UpdateEffectBounds(bounds, current_transform_);
  PopState();
}
