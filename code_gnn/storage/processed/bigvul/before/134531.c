void WebContentsViewAura::PrepareContentWindowForOverscroll() {
  StopObservingImplicitAnimations();
  aura::Window* content = GetContentNativeView();
  content->layer()->GetAnimator()->AbortAllAnimations();
  content->SetTransform(gfx::Transform());
  content->layer()->SetLayerBrightness(0.f);
}
