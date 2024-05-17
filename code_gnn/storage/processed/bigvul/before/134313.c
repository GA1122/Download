void TabStrip::RemoveTabDelegate::AnimationEnded(
    const gfx::Animation* animation) {
  CompleteRemove();
}
