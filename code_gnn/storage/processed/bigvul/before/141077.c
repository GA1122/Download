AnimationClock& Document::GetAnimationClock() {
  DCHECK(GetPage());
  return GetPage()->Animator().Clock();
}
