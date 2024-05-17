SkBlendMode PropertyTreeManager::SynthesizeCcEffectsForClipsIfNeeded(
    const ClipPaintPropertyNode* target_clip,
    SkBlendMode delegated_blend,
    bool effect_is_newly_built) {
  if (delegated_blend != SkBlendMode::kSrcOver) {
    while (IsCurrentCcEffectSynthetic())
      CloseCcEffect();

    GetEffectTree().Node(current_effect_id_)->has_render_surface = true;
  } else {
    const auto& lca = LowestCommonAncestor(*current_clip_, *target_clip);
    while (current_clip_ != &lca) {
      DCHECK(IsCurrentCcEffectSynthetic());
      const auto* pre_exit_clip = current_clip_;
      CloseCcEffect();
      if (IsNodeOnAncestorChain(lca, *pre_exit_clip, *current_clip_))
        break;
    }

    if (!effect_is_newly_built && !IsCurrentCcEffectSynthetic() &&
        current_effect_->Opacity() != 1.f)
      GetEffectTree().Node(current_effect_id_)->has_render_surface = true;
  }

  DCHECK(current_clip_->IsAncestorOf(*target_clip));

  Vector<const ClipPaintPropertyNode*> pending_clips;
  for (; target_clip != current_clip_; target_clip = target_clip->Parent()) {
    DCHECK(target_clip);
    bool should_synthesize =
        target_clip->ClipRect().IsRounded() || target_clip->ClipPath();
    if (should_synthesize)
      pending_clips.push_back(target_clip);
  }

  for (size_t i = pending_clips.size(); i--;) {
    const ClipPaintPropertyNode* next_clip = pending_clips[i];

    cc::EffectNode& mask_isolation = *GetEffectTree().Node(
        GetEffectTree().Insert(cc::EffectNode(), current_effect_id_));
    mask_isolation.clip_id = EnsureCompositorClipNode(next_clip);
    mask_isolation.has_render_surface = true;
    DCHECK(delegated_blend != SkBlendMode::kDstIn);
    mask_isolation.blend_mode = delegated_blend;
    delegated_blend = SkBlendMode::kSrcOver;

    effect_stack_.emplace_back(
        EffectStackEntry{current_effect_id_, current_effect_type_,
                         current_effect_, current_clip_});
    current_effect_id_ = mask_isolation.id;
    current_effect_type_ = CcEffectType::kSynthesizedClip;
    current_clip_ = next_clip;
  }

  return delegated_blend;
}
