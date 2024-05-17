void WebContentsViewAura::UpdateOverscrollWindowBrightness(float delta_x) {
  if (!overscroll_change_brightness_)
    return;

  const float kBrightnessMin = -.1f;
  const float kBrightnessMax = -.01f;

  float ratio = fabs(delta_x) / GetViewBounds().width();
  ratio = std::min(1.f, ratio);
  if (base::i18n::IsRTL())
    ratio = 1.f - ratio;
  float brightness = current_overscroll_gesture_ == OVERSCROLL_WEST ?
      kBrightnessMin + ratio * (kBrightnessMax - kBrightnessMin) :
      kBrightnessMax - ratio * (kBrightnessMax - kBrightnessMin);
  brightness = std::max(kBrightnessMin, brightness);
  brightness = std::min(kBrightnessMax, brightness);
  aura::Window* window = GetWindowToAnimateForOverscroll();
  window->layer()->SetLayerBrightness(brightness);
}
