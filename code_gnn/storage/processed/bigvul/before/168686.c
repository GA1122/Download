void UiSceneCreator::CreateSystemIndicators() {
  struct Indicator {
    UiElementName name;
    const gfx::VectorIcon& icon;
    int resource_string;
    bool PermissionsModel::*signal;
  };
  const std::vector<Indicator> indicators = {
      {kAudioCaptureIndicator, vector_icons::kMicrophoneIcon,
       IDS_AUDIO_CALL_NOTIFICATION_TEXT_2,
       &PermissionsModel::audio_capture_enabled},
      {kVideoCaptureIndicator, vector_icons::kVideocamIcon,
       IDS_VIDEO_CALL_NOTIFICATION_TEXT_2,
       &PermissionsModel::video_capture_enabled},
      {kScreenCaptureIndicator, vector_icons::kScreenShareIcon,
       IDS_SCREEN_CAPTURE_NOTIFICATION_TEXT_2,
       &PermissionsModel::screen_capture_enabled},
      {kBluetoothConnectedIndicator, vector_icons::kBluetoothConnectedIcon, 0,
       &PermissionsModel::bluetooth_connected},
      {kLocationAccessIndicator, vector_icons::kLocationOnIcon, 0,
       &PermissionsModel::location_access},
  };

  std::unique_ptr<LinearLayout> indicator_layout =
      base::MakeUnique<LinearLayout>(LinearLayout::kRight);
  indicator_layout->SetName(kIndicatorLayout);
  indicator_layout->set_hit_testable(false);
  indicator_layout->set_y_anchoring(TOP);
  indicator_layout->SetTranslate(0, kIndicatorVerticalOffset,
                                 kIndicatorDistanceOffset);
  indicator_layout->set_margin(kIndicatorGap);
  indicator_layout->AddBinding(
      VR_BIND_FUNC(bool, Model, model_, fullscreen == false, UiElement,
                   indicator_layout.get(), SetVisible));
  scene_->AddUiElement(k2dBrowsingContentGroup, std::move(indicator_layout));

  for (const auto& indicator : indicators) {
    auto element = base::MakeUnique<SystemIndicator>(512);
    element->GetDerivedTexture()->SetIcon(indicator.icon);
    element->GetDerivedTexture()->SetMessageId(indicator.resource_string);
    element->SetName(indicator.name);
    element->SetDrawPhase(kPhaseForeground);
    element->set_requires_layout(false);
    element->SetSize(0, kIndicatorHeight);
    element->SetVisible(false);
    BindColor(model_, element.get(), &ColorScheme::system_indicator_background,
              &TexturedElement::SetBackgroundColor);
    BindColor(model_, element.get(), &ColorScheme::system_indicator_foreground,
              &TexturedElement::SetForegroundColor);
    element->AddBinding(base::MakeUnique<Binding<bool>>(
        base::Bind(
            [](Model* m, bool PermissionsModel::*permission) {
              return m->permissions.*permission;
            },
            base::Unretained(model_), indicator.signal),
        base::Bind(
            [](UiElement* e, const bool& v) {
              e->SetVisible(v);
              e->set_requires_layout(v);
            },
            base::Unretained(element.get()))));
    scene_->AddUiElement(kIndicatorLayout, std::move(element));
  }
}
