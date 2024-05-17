void UiSceneCreator::CreateWebVRExitWarning() {
  auto scrim = base::MakeUnique<FullScreenRect>();
  scrim->SetName(kScreenDimmer);
  scrim->SetDrawPhase(kPhaseOverlayBackground);
  scrim->SetVisible(false);
  scrim->set_hit_testable(false);
  scrim->SetOpacity(kScreenDimmerOpacity);
  scrim->SetCenterColor(model_->color_scheme().dimmer_inner);
  scrim->SetEdgeColor(model_->color_scheme().dimmer_outer);
  scrim->AddBinding(VR_BIND_FUNC(bool, Model, model_, exiting_vr, UiElement,
                                 scrim.get(), SetVisible));
  scene_->AddUiElement(k2dBrowsingRoot, std::move(scrim));

  auto scaler = base::MakeUnique<ScaledDepthAdjuster>(kExitWarningDistance);
  auto exit_warning_text = base::MakeUnique<Text>(kExitWarningFontHeightDMM);
  exit_warning_text->SetName(kExitWarningText);
  exit_warning_text->SetDrawPhase(kPhaseOverlayForeground);
  exit_warning_text->SetText(
      l10n_util::GetStringUTF16(IDS_VR_BROWSER_UNSUPPORTED_PAGE));
  exit_warning_text->SetSize(kExitWarningTextWidthDMM, 0);
  exit_warning_text->SetVisible(true);
  exit_warning_text->set_hit_testable(false);
  BindColor(model_, exit_warning_text.get(),
            &ColorScheme::exit_warning_foreground, &Text::SetColor);

  auto exit_warning_bg = base::MakeUnique<Rect>();
  exit_warning_bg->SetName(kExitWarningBackground);
  exit_warning_bg->SetDrawPhase(kPhaseOverlayForeground);
  exit_warning_bg->set_bounds_contain_children(true);
  exit_warning_bg->set_padding(kExitWarningXPaddingDMM,
                               kExitWarningYPaddingDMM);
  exit_warning_bg->set_corner_radius(kExitWarningCornerRadiusDMM);
  exit_warning_bg->set_hit_testable(false);
  exit_warning_bg->AddChild(std::move(exit_warning_text));
  exit_warning_bg->AddBinding(VR_BIND_FUNC(bool, Model, model_, exiting_vr,
                                           UiElement, exit_warning_bg.get(),
                                           SetVisible));
  BindColor(model_, exit_warning_bg.get(),
            &ColorScheme::exit_warning_background, &Rect::SetColor);
  scaler->AddChild(std::move(exit_warning_bg));
  scene_->AddUiElement(k2dBrowsingViewportAwareRoot, std::move(scaler));
}
