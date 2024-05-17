void ConversionContext::SwitchToClip(const ClipPaintPropertyNode* target_clip) {
  if (target_clip == current_clip_)
    return;

  const ClipPaintPropertyNode* lca_clip =
      &LowestCommonAncestor(*target_clip, *current_clip_);
  while (current_clip_ != lca_clip) {
#if DCHECK_IS_ON()
    DCHECK(state_stack_.size() && state_stack_.back().type == StateEntry::kClip)
        << "Error: Chunk has a clip that escaped its layer's or effect's clip."
        << "\ntarget_clip:\n"
        << target_clip->ToTreeString().Utf8().data() << "current_clip_:\n"
        << current_clip_->ToTreeString().Utf8().data();
#endif
    if (!state_stack_.size() || state_stack_.back().type != StateEntry::kClip)
      break;
    current_clip_ = current_clip_->Parent();
    StateEntry& previous_state = state_stack_.back();
    if (current_clip_ == lca_clip) {
      current_clip_ = lca_clip = previous_state.clip;
    }
    if (current_clip_ == previous_state.clip)
      EndClip();
  }

  if (target_clip == current_clip_)
    return;

  Vector<const ClipPaintPropertyNode*, 1u> pending_clips;
  for (const ClipPaintPropertyNode* clip = target_clip; clip != current_clip_;
       clip = clip->Parent()) {
    if (!clip)
      break;
    pending_clips.push_back(clip);
  }

  DCHECK(pending_clips.size());
  auto pending_combined_clip_rect = pending_clips.back()->ClipRect();
  const auto* lowest_combined_clip_node = pending_clips.back();
  for (size_t i = pending_clips.size() - 1; i--;) {
    const auto* sub_clip = pending_clips[i];
    if (CombineClip(sub_clip, pending_combined_clip_rect)) {
      lowest_combined_clip_node = sub_clip;
    } else {
      StartClip(pending_combined_clip_rect, lowest_combined_clip_node);
      pending_combined_clip_rect = sub_clip->ClipRect();
      lowest_combined_clip_node = sub_clip;
    }
  }
  StartClip(pending_combined_clip_rect, lowest_combined_clip_node);

  DCHECK_EQ(current_clip_, target_clip);
}
