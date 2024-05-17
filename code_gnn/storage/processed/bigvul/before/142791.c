void HTMLMediaElement::EnsureMediaControls() {
  if (GetMediaControls())
    return;

  ShadowRoot& shadow_root = EnsureUserAgentShadowRoot();
  media_controls_ =
      CoreInitializer::GetInstance().CreateMediaControls(*this, shadow_root);

  AssertShadowRootChildren(shadow_root);
}
