void UiSceneCreator::CreateCloseButton() {
  base::Callback<void()> click_handler = base::Bind(
      [](Model* model, UiBrowserInterface* browser) {
        if (model->fullscreen) {
          browser->ExitFullscreen();
        }
        if (model->in_cct) {
          browser->ExitCct();
        }
      },
      base::Unretained(model_), base::Unretained(browser_));
  std::unique_ptr<DiscButton> element =
      Create<DiscButton>(kCloseButton, kPhaseForeground, click_handler,
                         vector_icons::kClose16Icon);
  element->SetSize(kCloseButtonWidth, kCloseButtonHeight);
  element->set_hover_offset(kButtonZOffsetHoverDMM * kCloseButtonDistance);
  element->SetTranslate(0, kCloseButtonVerticalOffset, -kCloseButtonDistance);
  BindButtonColors(model_, element.get(), &ColorScheme::button_colors,
                   &DiscButton::SetButtonColors);

  element->AddBinding(
      VR_BIND_FUNC(bool, Model, model_,
                   browsing_mode() && (model->fullscreen || model->in_cct),
                   UiElement, element.get(), SetVisible));
  element->AddBinding(
      VR_BIND(bool, Model, model_, fullscreen, UiElement, element.get(),
              SetTranslate(0,
                           value ? kCloseButtonFullscreenVerticalOffset
                                 : kCloseButtonVerticalOffset,
                           value ? -kCloseButtonFullscreenDistance
                                 : -kCloseButtonDistance)));
  element->AddBinding(VR_BIND(
      bool, Model, model_, fullscreen, UiElement, element.get(),
      SetSize(value ? kCloseButtonFullscreenWidth : kCloseButtonWidth,
              value ? kCloseButtonFullscreenHeight : kCloseButtonHeight)));

  scene_->AddUiElement(k2dBrowsingForeground, std::move(element));
}
