void UiSceneCreator::CreateUrlBar() {
  auto scaler = base::MakeUnique<ScaledDepthAdjuster>(kUrlBarDistance);
  scaler->SetName(kUrlBarDmmRoot);
  scene_->AddUiElement(k2dBrowsingForeground, std::move(scaler));

  base::RepeatingCallback<void()> url_click_callback;
  if (base::FeatureList::IsEnabled(features::kVrBrowserKeyboard)) {
    url_click_callback = base::BindRepeating(
        [](Model* m, UiBrowserInterface* browser) {
          m->omnibox_input_active = true;
        },
        base::Unretained(model_), base::Unretained(browser_));
  } else {
    url_click_callback = base::BindRepeating([] {});
  }

  auto url_bar = base::MakeUnique<UrlBar>(
      512,
      base::BindRepeating(&UiBrowserInterface::NavigateBack,
                          base::Unretained(browser_)),
      url_click_callback,
      base::BindRepeating(&UiBrowserInterface::OnUnsupportedMode,
                          base::Unretained(browser_)));
  url_bar->SetName(kUrlBar);
  url_bar->SetDrawPhase(kPhaseForeground);
  url_bar->SetTranslate(0, kUrlBarVerticalOffsetDMM, 0);
  url_bar->SetRotate(1, 0, 0, kUrlBarRotationRad);
  url_bar->SetSize(kUrlBarWidthDMM, kUrlBarHeightDMM);
  url_bar->AddBinding(VR_BIND_FUNC(bool, Model, model_, fullscreen == false,
                                   UiElement, url_bar.get(), SetVisible));
  url_bar->AddBinding(VR_BIND_FUNC(ToolbarState, Model, model_, toolbar_state,
                                   UrlBar, url_bar.get(), SetToolbarState));
  url_bar->AddBinding(VR_BIND_FUNC(UrlBarColors, Model, model_,
                                   color_scheme().url_bar, UrlBar,
                                   url_bar.get(), SetColors));
  url_bar->AddBinding(VR_BIND_FUNC(bool, Model, model_, can_navigate_back,
                                   UrlBar, url_bar.get(),
                                   SetHistoryButtonsEnabled));
  BindColor(model_, url_bar.get(), &ColorScheme::element_background,
            &TexturedElement::SetBackgroundColor);
  scene_->AddUiElement(kUrlBarDmmRoot, std::move(url_bar));

  auto indicator_bg = base::MakeUnique<Rect>();
  indicator_bg->SetName(kLoadingIndicator);
  indicator_bg->SetDrawPhase(kPhaseForeground);
  indicator_bg->SetTranslate(0, kLoadingIndicatorVerticalOffsetDMM, 0);
  indicator_bg->SetSize(kLoadingIndicatorWidthDMM, kLoadingIndicatorHeightDMM);
  indicator_bg->set_y_anchoring(TOP);
  indicator_bg->SetTransitionedProperties({OPACITY});
  indicator_bg->set_corner_radius(kLoadingIndicatorHeightDMM * 0.5f);
  indicator_bg->AddBinding(VR_BIND_FUNC(bool, Model, model_, loading, Rect,
                                        indicator_bg.get(), SetVisible));
  BindColor(model_, indicator_bg.get(),
            &ColorScheme::loading_indicator_background, &Rect::SetColor);

  scene_->AddUiElement(kUrlBar, std::move(indicator_bg));

  auto indicator_fg = base::MakeUnique<Rect>();
  indicator_fg->SetDrawPhase(kPhaseForeground);
  indicator_fg->SetName(kLoadingIndicatorForeground);
  indicator_fg->set_x_anchoring(LEFT);
  indicator_fg->set_corner_radius(kLoadingIndicatorHeightDMM * 0.5f);
  indicator_fg->set_hit_testable(false);
  BindColor(model_, indicator_fg.get(),
            &ColorScheme::loading_indicator_foreground, &Rect::SetColor);
  indicator_fg->AddBinding(base::MakeUnique<Binding<float>>(
      base::Bind([](Model* m) { return m->load_progress; },
                 base::Unretained(model_)),
      base::Bind(
          [](Rect* r, const float& value) {
            r->SetSize(kLoadingIndicatorWidthDMM * value,
                       kLoadingIndicatorHeightDMM);
            r->SetTranslate(kLoadingIndicatorWidthDMM * value * 0.5f, 0.0f,
                            0.001f);
          },
          base::Unretained(indicator_fg.get()))));
  scene_->AddUiElement(kLoadingIndicator, std::move(indicator_fg));
}
