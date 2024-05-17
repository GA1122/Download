int PropertyTreeManager::SwitchToEffectNodeWithSynthesizedClip(
    const EffectPaintPropertyNode& next_effect,
    const ClipPaintPropertyNode& next_clip) {
  const auto& ancestor = LowestCommonAncestor(*current_effect_, next_effect);
  while (current_effect_ != &ancestor)
    CloseCcEffect();

  bool newly_built = BuildEffectNodesRecursively(&next_effect);
  SynthesizeCcEffectsForClipsIfNeeded(&next_clip, SkBlendMode::kSrcOver,
                                      newly_built);

  return current_effect_id_;
}
