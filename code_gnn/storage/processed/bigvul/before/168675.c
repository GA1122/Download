void UiSceneCreator::CreateAudioPermissionPrompt() {
  std::unique_ptr<UiElement> element;

  auto backplane = base::MakeUnique<InvisibleHitTarget>();
  backplane->SetDrawPhase(kPhaseForeground);
  backplane->SetName(kAudioPermissionPromptBackplane);
  backplane->SetSize(kPromptBackplaneSize, kPromptBackplaneSize);
  backplane->SetTranslate(0.0, kContentVerticalOffset, -kOverlayPlaneDistance);
  EventHandlers event_handlers;
  event_handlers.button_up = base::Bind(
      [](UiBrowserInterface* browser, Model* m) {
        browser->OnExitVrPromptResult(
            ExitVrPromptChoice::CHOICE_NONE,
            GetReasonForPrompt(m->active_modal_prompt_type));
      },
      base::Unretained(browser_), base::Unretained(model_));
  backplane->set_event_handlers(event_handlers);
  backplane->SetVisible(false);
  backplane->SetTransitionedProperties({OPACITY});
  backplane->AddBinding(VR_BIND_FUNC(
      bool, Model, model_,
      active_modal_prompt_type ==
          kModalPromptTypeExitVRForVoiceSearchRecordAudioOsPermission,
      UiElement, backplane.get(), SetVisible));

  std::unique_ptr<Shadow> shadow = base::MakeUnique<Shadow>();
  shadow->SetName(kAudioPermissionPromptShadow);
  shadow->SetDrawPhase(kPhaseForeground);

  std::unique_ptr<AudioPermissionPrompt> prompt =
      base::MakeUnique<AudioPermissionPrompt>(
          1024,
          base::Bind(
              &UiBrowserInterface::OnExitVrPromptResult,
              base::Unretained(browser_), ExitVrPromptChoice::CHOICE_EXIT,
              UiUnsupportedMode::kVoiceSearchNeedsRecordAudioOsPermission),
          base::Bind(
              &UiBrowserInterface::OnExitVrPromptResult,
              base::Unretained(browser_), ExitVrPromptChoice::CHOICE_STAY,
              UiUnsupportedMode::kVoiceSearchNeedsRecordAudioOsPermission));
  prompt->SetName(kAudioPermissionPrompt);
  prompt->SetDrawPhase(kPhaseForeground);
  prompt->SetSize(kAudioPermissionPromptWidth, kAudioPermissionPromptHeight);
  prompt->SetTranslate(0.0, 0.0f, kAudionPermisionPromptDepth);
  BindButtonColors(model_, prompt.get(),
                   &ColorScheme::audio_permission_prompt_primary_button_colors,
                   &AudioPermissionPrompt::SetPrimaryButtonColors);
  BindButtonColors(
      model_, prompt.get(),
      &ColorScheme::audio_permission_prompt_secondary_button_colors,
      &AudioPermissionPrompt::SetSecondaryButtonColors);
  BindColor(model_, prompt.get(),
            &ColorScheme::audio_permission_prompt_icon_foreground,
            &AudioPermissionPrompt::SetIconColor);
  BindColor(model_, prompt.get(),
            &ColorScheme::audio_permission_prompt_background,
            &TexturedElement::SetBackgroundColor);
  BindColor(model_, prompt.get(), &ColorScheme::element_foreground,
            &TexturedElement::SetForegroundColor);
  shadow->AddChild(std::move(prompt));
  backplane->AddChild(std::move(shadow));
  scene_->AddUiElement(k2dBrowsingRoot, std::move(backplane));
}
