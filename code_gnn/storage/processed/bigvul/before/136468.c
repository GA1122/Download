bool PropertyTreeManager::BuildEffectNodesRecursively(
    const EffectPaintPropertyNode* next_effect) {
  if (next_effect == current_effect_)
    return false;
  DCHECK(next_effect);

  bool newly_built = BuildEffectNodesRecursively(next_effect->Parent());
  DCHECK_EQ(next_effect->Parent(), current_effect_);

#if DCHECK_IS_ON()
  DCHECK(!effect_nodes_converted_.Contains(next_effect))
      << "Malformed paint artifact. Paint chunks under the same effect should "
         "be contiguous.";
  effect_nodes_converted_.insert(next_effect);
#endif

  SkBlendMode used_blend_mode;
  int output_clip_id;
  if (next_effect->OutputClip()) {
    used_blend_mode = SynthesizeCcEffectsForClipsIfNeeded(
        next_effect->OutputClip(), next_effect->BlendMode(), newly_built);
    output_clip_id = EnsureCompositorClipNode(next_effect->OutputClip());
  } else {
    while (IsCurrentCcEffectSynthetic())
      CloseCcEffect();
    if (next_effect->BlendMode() != SkBlendMode::kSrcOver ||
        (!newly_built && current_effect_->Opacity() != 1.f))
      GetEffectTree().Node(current_effect_id_)->has_render_surface = true;

    used_blend_mode = next_effect->BlendMode();
    output_clip_id = GetEffectTree().Node(current_effect_id_)->clip_id;
  }

  cc::EffectNode& effect_node = *GetEffectTree().Node(
      GetEffectTree().Insert(cc::EffectNode(), current_effect_id_));
  effect_node.stable_id =
      next_effect->GetCompositorElementId().ToInternalValue();
  effect_node.clip_id = output_clip_id;
  if (!next_effect->Filter().IsEmpty() ||
      used_blend_mode != SkBlendMode::kSrcOver)
    effect_node.has_render_surface = true;
  effect_node.opacity = next_effect->Opacity();
  if (next_effect->GetColorFilter() != kColorFilterNone) {
    DCHECK(next_effect->GetColorFilter() == kColorFilterLuminanceToAlpha);
    DCHECK(used_blend_mode == SkBlendMode::kDstIn);
    DCHECK(next_effect->Filter().IsEmpty());
    effect_node.filters.Append(cc::FilterOperation::CreateReferenceFilter(
        sk_make_sp<ColorFilterPaintFilter>(SkLumaColorFilter::Make(),
                                           nullptr)));
  } else {
    effect_node.filters = next_effect->Filter().AsCcFilterOperations();
    effect_node.filters_origin = next_effect->PaintOffset();
    effect_node.transform_id =
        EnsureCompositorTransformNode(next_effect->LocalTransformSpace());
  }
  effect_node.blend_mode = used_blend_mode;
  CompositorElementId compositor_element_id =
      next_effect->GetCompositorElementId();
  if (compositor_element_id) {
    DCHECK(property_trees_.element_id_to_effect_node_index.find(
               compositor_element_id) ==
           property_trees_.element_id_to_effect_node_index.end());
    property_trees_.element_id_to_effect_node_index[compositor_element_id] =
        effect_node.id;
  }
  effect_stack_.emplace_back(EffectStackEntry{current_effect_id_,
                                              current_effect_type_,
                                              current_effect_, current_clip_});
  current_effect_id_ = effect_node.id;
  current_effect_type_ = CcEffectType::kEffect;
  current_effect_ = next_effect;
  if (next_effect->OutputClip())
    current_clip_ = next_effect->OutputClip();

  return true;
}
