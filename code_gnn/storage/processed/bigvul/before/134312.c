void TabStrip::RemoveTabDelegate::AnimationCanceled(
    const gfx::Animation* animation) {
  CompleteRemove();
}
