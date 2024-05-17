bool LayerTreeHostImpl::AnimateBrowserControls(base::TimeTicks time) {
  if (!browser_controls_offset_manager_->has_animation())
    return false;

  gfx::Vector2dF scroll = browser_controls_offset_manager_->Animate(time);

  if (browser_controls_offset_manager_->has_animation())
    SetNeedsOneBeginImplFrame();

  if (active_tree_->TotalScrollOffset().y() == 0.f)
    return false;

  if (scroll.IsZero())
    return false;

  DCHECK(viewport());
  viewport()->ScrollBy(scroll, gfx::Point(), false, false, true);
  client_->SetNeedsCommitOnImplThread();
  client_->RenewTreePriority();
  return true;
}
