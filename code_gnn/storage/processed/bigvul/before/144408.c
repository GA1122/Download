void ExpandableContainerView::AnimationProgressed(
    const gfx::Animation* animation) {
  DCHECK_EQ(&slide_animation_, animation);
  if (details_view_)
    details_view_->AnimateToState(animation->GetCurrentValue());
}
