void PropertyTreeManager::CloseCcEffect() {
  DCHECK(effect_stack_.size());
  const EffectStackEntry& previous_state = effect_stack_.back();

  bool clear_synthetic_effects =
      !IsCurrentCcEffectSynthetic() &&
      current_effect_->BlendMode() != SkBlendMode::kSrcOver;

  if (IsCurrentCcEffectSynthetic())
    EmitClipMaskLayer();

  current_effect_id_ = previous_state.effect_id;
  current_effect_type_ = previous_state.effect_type;
  current_effect_ = previous_state.effect;
  current_clip_ = previous_state.clip;
  effect_stack_.pop_back();

  if (clear_synthetic_effects) {
    while (IsCurrentCcEffectSynthetic())
      CloseCcEffect();
  }
}
