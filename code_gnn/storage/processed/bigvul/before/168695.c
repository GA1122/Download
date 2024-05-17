void UiSceneCreator::CreateWebVrTimeoutScreen() {
  auto scaler = base::MakeUnique<ScaledDepthAdjuster>(kSpinnerDistance);

  auto timeout_message =
      Create<Rect>(kWebVrTimeoutMessage, kPhaseOverlayForeground);
  timeout_message->SetVisible(false);
  timeout_message->set_bounds_contain_children(true);
  timeout_message->set_corner_radius(kTimeoutMessageCornerRadiusDMM);
  timeout_message->SetTransitionedProperties({OPACITY, TRANSFORM});
  timeout_message->set_padding(kTimeoutMessageHorizontalPaddingDMM,
                               kTimeoutMessageVerticalPaddingDMM);
  timeout_message->AddBinding(
      VR_BIND_FUNC(bool, Model, model_, web_vr_timeout_state == kWebVrTimedOut,
                   Rect, timeout_message.get(), SetVisible));
  timeout_message->SetColor(model_->color_scheme().timeout_message_background);

  auto timeout_layout = Create<LinearLayout>(kWebVrTimeoutMessageLayout,
                                             kPhaseNone, LinearLayout::kRight);
  timeout_layout->set_hit_testable(false);
  timeout_layout->set_margin(kTimeoutMessageLayoutGapDMM);

  auto timeout_icon = Create<VectorIcon>(kWebVrTimeoutMessageIcon,
                                         kPhaseOverlayForeground, 512);
  timeout_icon->SetIcon(kSadTabIcon);
  timeout_icon->SetSize(kTimeoutMessageIconWidthDMM,
                        kTimeoutMessageIconHeightDMM);

  auto timeout_text =
      Create<Text>(kWebVrTimeoutMessageText, kPhaseOverlayForeground,
                   kTimeoutMessageTextFontHeightDMM);
  timeout_text->SetText(
      l10n_util::GetStringUTF16(IDS_VR_WEB_VR_TIMEOUT_MESSAGE));
  timeout_text->SetColor(model_->color_scheme().timeout_message_foreground);
  timeout_text->SetTextAlignment(UiTexture::kTextAlignmentLeft);
  timeout_text->SetSize(kTimeoutMessageTextWidthDMM,
                        kTimeoutMessageTextHeightDMM);

  auto button_scaler =
      base::MakeUnique<ScaledDepthAdjuster>(kTimeoutButtonDepthOffset);

  auto button = Create<DiscButton>(
      kWebVrTimeoutMessageButton, kPhaseOverlayForeground,
      base::Bind(&UiBrowserInterface::ExitPresent, base::Unretained(browser_)),
      vector_icons::kClose16Icon);
  button->SetVisible(false);
  button->SetTranslate(0, -kTimeoutMessageTextWidthDMM, 0);
  button->SetRotate(1, 0, 0, kTimeoutButtonRotationRad);
  button->SetTransitionedProperties({OPACITY});
  button->SetSize(kWebVrTimeoutMessageButtonDiameterDMM,
                  kWebVrTimeoutMessageButtonDiameterDMM);
  button->AddBinding(VR_BIND_FUNC(bool, Model, model_,
                                  web_vr_timeout_state == kWebVrTimedOut,
                                  DiscButton, button.get(), SetVisible));
  BindButtonColors(model_, button.get(), &ColorScheme::button_colors,
                   &DiscButton::SetButtonColors);

  auto timeout_button_text =
      Create<Text>(kWebVrTimeoutMessageButtonText, kPhaseOverlayForeground,
                   kTimeoutMessageTextFontHeightDMM);

  timeout_button_text->SetText(
      l10n_util::GetStringUTF16(IDS_VR_WEB_VR_EXIT_BUTTON_LABEL));
  timeout_button_text->SetColor(model_->color_scheme().spinner_color);
  timeout_button_text->SetSize(kTimeoutButtonTextWidthDMM,
                               kTimeoutButtonTextHeightDMM);
  timeout_button_text->set_y_anchoring(BOTTOM);
  timeout_button_text->SetTranslate(0, -kTimeoutButtonTextVerticalOffsetDMM, 0);

  button->AddChild(std::move(timeout_button_text));
  timeout_layout->AddChild(std::move(timeout_icon));
  timeout_layout->AddChild(std::move(timeout_text));
  timeout_message->AddChild(std::move(timeout_layout));
  button_scaler->AddChild(std::move(button));
  timeout_message->AddChild(std::move(button_scaler));
  scaler->AddChild(std::move(timeout_message));
  scene_->AddUiElement(kSplashScreenViewportAwareRoot, std::move(scaler));
}
