RenderWidgetHostViewAura::CreateSyntheticGestureTarget() {
  return scoped_ptr<SyntheticGestureTarget>(
      new SyntheticGestureTargetAura(host_));
}
