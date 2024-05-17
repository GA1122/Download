void ConversionContext::SwitchToEffect(
    const EffectPaintPropertyNode* target_effect) {
  if (target_effect == current_effect_)
    return;

  const EffectPaintPropertyNode* lca_effect =
      &LowestCommonAncestor(*target_effect, *current_effect_);
  while (current_effect_ != lca_effect) {
    EndClips();
#if DCHECK_IS_ON()
    DCHECK(state_stack_.size())
        << "Error: Chunk has an effect that escapes layer's effect.\n"
        << "target_effect:\n"
        << target_effect->ToTreeString().Utf8().data() << "current_effect_:\n"
        << current_effect_->ToTreeString().Utf8().data();
#endif
    if (!state_stack_.size())
      break;
    EndEffect();
  }

  Vector<const EffectPaintPropertyNode*, 1u> pending_effects;
  for (const EffectPaintPropertyNode* effect = target_effect;
       effect != current_effect_; effect = effect->Parent()) {
    if (!effect)
      break;
    pending_effects.push_back(effect);
  }

  for (size_t i = pending_effects.size(); i--;) {
    const EffectPaintPropertyNode* sub_effect = pending_effects[i];
    DCHECK_EQ(current_effect_, sub_effect->Parent());
    StartEffect(sub_effect);
  }
}
