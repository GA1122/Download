bool PaintArtifactCompositor::CanDecompositeEffect(
    const EffectPaintPropertyNode* effect,
    const PendingLayer& layer) {
  if (layer.property_tree_state.Effect() != effect)
    return false;
  if (layer.requires_own_layer)
    return false;
  if (effect->BlendMode() != SkBlendMode::kSrcOver)
    return false;
  if (effect->HasDirectCompositingReasons())
    return false;
  if (!CanUpcastTo(layer.property_tree_state,
                   PropertyTreeState(effect->LocalTransformSpace(),
                                     effect->OutputClip()
                                         ? effect->OutputClip()
                                         : layer.property_tree_state.Clip(),
                                     effect)))
    return false;
  return true;
}
