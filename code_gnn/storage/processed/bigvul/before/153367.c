void ResetDraggingStateDelegate::AnimationEnded(
    const gfx::Animation* animation) {
  tab()->set_dragging(false);
  AnimationProgressed(animation);   
}
