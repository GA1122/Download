void UiSceneCreator::CreateVoiceSearchUiGroup() {
  auto voice_search_button = Create<DiscButton>(
      kVoiceSearchButton, kPhaseForeground,
      base::BindRepeating(&UiBrowserInterface::SetVoiceSearchActive,
                          base::Unretained(browser_), true),
      vector_icons::kMicrophoneIcon);
  voice_search_button->SetSize(kVoiceSearchButtonDiameterDMM,
                               kVoiceSearchButtonDiameterDMM);
  voice_search_button->set_hover_offset(kButtonZOffsetHoverDMM);
  voice_search_button->SetTranslate(0.f, -kVoiceSearchButtonYOffsetDMM, 0.f);
  voice_search_button->set_y_anchoring(BOTTOM);
  voice_search_button->set_y_centering(TOP);
  voice_search_button->AddBinding(base::MakeUnique<Binding<bool>>(
      base::Bind(
          [](Model* m) {
            return !m->incognito &&
                   m->speech.has_or_can_request_audio_permission;
          },
          base::Unretained(model_)),
      base::Bind([](UiElement* e, const bool& v) { e->SetVisible(v); },
                 voice_search_button.get())));
  BindButtonColors(model_, voice_search_button.get(),
                   &ColorScheme::button_colors, &DiscButton::SetButtonColors);
  scene_->AddUiElement(kUrlBar, std::move(voice_search_button));

  auto speech_recognition_root = base::MakeUnique<UiElement>();
  speech_recognition_root->SetName(kSpeechRecognitionRoot);
  speech_recognition_root->SetTranslate(0.f, 0.f, -kContentDistance);
  speech_recognition_root->set_hit_testable(false);
  scene_->AddUiElement(k2dBrowsingRoot, std::move(speech_recognition_root));

  TransientElement* speech_result_parent =
      AddTransientParent(kSpeechRecognitionResult, kSpeechRecognitionRoot,
                         kSpeechRecognitionResultTimeoutSeconds, false, scene_);
  speech_result_parent->SetVisibleImmediately(false);
  speech_result_parent->SetTransitionedProperties({OPACITY});
  speech_result_parent->SetTransitionDuration(base::TimeDelta::FromMilliseconds(
      kSpeechRecognitionOpacityAnimationDurationMs));
  speech_result_parent->AddBinding(base::MakeUnique<Binding<bool>>(
      base::Bind([](Model* m) { return m->speech.recognition_result.empty(); },
                 base::Unretained(model_)),
      base::Bind(
          [](UiElement* e, const bool& v) {
            if (v) {
              e->SetVisible(false);
            } else {
              e->SetVisibleImmediately(true);
            }
          },
          speech_result_parent)));
  auto speech_result =
      base::MakeUnique<Text>(kVoiceSearchRecognitionResultTextHeight);
  speech_result->SetName(kSpeechRecognitionResultText);
  speech_result->SetDrawPhase(kPhaseForeground);
  speech_result->SetTranslate(0.f, kSpeechRecognitionResultTextYOffset, 0.f);
  speech_result->set_hit_testable(false);
  speech_result->SetSize(kVoiceSearchRecognitionResultTextWidth, 0);
  speech_result->SetTextAlignment(UiTexture::kTextAlignmentCenter);
  BindColor(model_, speech_result.get(), &ColorScheme::prompt_foreground,
            &Text::SetColor);
  speech_result->AddBinding(VR_BIND_FUNC(base::string16, Model, model_,
                                         speech.recognition_result, Text,
                                         speech_result.get(), SetText));
  speech_result_parent->AddChild(std::move(speech_result));

  auto circle = base::MakeUnique<Rect>();
  circle->SetName(kSpeechRecognitionResultCircle);
  circle->SetDrawPhase(kPhaseForeground);
  circle->SetSize(kCloseButtonWidth * 2, kCloseButtonHeight * 2);
  circle->set_corner_radius(kCloseButtonWidth);
  circle->set_hit_testable(false);
  BindColor(model_, circle.get(),
            &ColorScheme::speech_recognition_circle_background,
            &Rect::SetColor);
  scene_->AddUiElement(kSpeechRecognitionResult, std::move(circle));

  auto microphone = base::MakeUnique<VectorIcon>(512);
  microphone->SetName(kSpeechRecognitionResultMicrophoneIcon);
  microphone->SetIcon(vector_icons::kMicrophoneIcon);
  microphone->SetDrawPhase(kPhaseForeground);
  microphone->set_hit_testable(false);
  microphone->SetSize(kCloseButtonWidth, kCloseButtonHeight);
  scene_->AddUiElement(kSpeechRecognitionResult, std::move(microphone));

  auto hit_target = base::MakeUnique<InvisibleHitTarget>();
  hit_target->SetName(kSpeechRecognitionResultBackplane);
  hit_target->SetDrawPhase(kPhaseForeground);
  hit_target->SetSize(kPromptBackplaneSize, kPromptBackplaneSize);
  scene_->AddUiElement(kSpeechRecognitionResult, std::move(hit_target));

  auto speech_recognition_listening = base::MakeUnique<UiElement>();
  UiElement* listening_ui_root = speech_recognition_listening.get();
  speech_recognition_listening->SetName(kSpeechRecognitionListening);
  speech_recognition_listening->set_hit_testable(false);
  speech_recognition_listening->SetVisibleImmediately(false);
  speech_recognition_listening->SetTransitionedProperties({OPACITY});
  speech_recognition_listening->SetTransitionDuration(
      base::TimeDelta::FromMilliseconds(
          kSpeechRecognitionOpacityAnimationDurationMs));
  speech_recognition_listening->AddBinding(base::MakeUnique<Binding<bool>>(
      base::Bind([](Model* m) { return m->speech.recognizing_speech; },
                 base::Unretained(model_)),
      base::Bind(
          [](UiElement* listening, UiElement* result, const bool& value) {
            if (result->GetTargetOpacity() != 0.f && !value) {
              listening->SetVisibleImmediately(false);
            } else {
              listening->SetVisible(value);
            }
          },
          base::Unretained(listening_ui_root),
          base::Unretained(speech_result_parent))));
  scene_->AddUiElement(kSpeechRecognitionRoot,
                       std::move(speech_recognition_listening));

  auto growing_circle = base::MakeUnique<Throbber>();
  growing_circle->SetName(kSpeechRecognitionListeningGrowingCircle);
  growing_circle->SetDrawPhase(kPhaseForeground);
  growing_circle->SetSize(kCloseButtonWidth * 2, kCloseButtonHeight * 2);
  growing_circle->set_corner_radius(kCloseButtonWidth);
  growing_circle->set_hit_testable(false);
  BindColor(model_, growing_circle.get(),
            &ColorScheme::speech_recognition_circle_background,
            &Rect::SetColor);
  growing_circle->AddBinding(VR_BIND(
      int, Model, model_, speech.speech_recognition_state, Throbber,
      growing_circle.get(),
      SetCircleGrowAnimationEnabled(value == SPEECH_RECOGNITION_IN_SPEECH ||
                                    value == SPEECH_RECOGNITION_RECOGNIZING ||
                                    value == SPEECH_RECOGNITION_READY)));
  scene_->AddUiElement(kSpeechRecognitionListening, std::move(growing_circle));

  auto inner_circle = base::MakeUnique<Rect>();
  inner_circle->SetName(kSpeechRecognitionListeningInnerCircle);
  inner_circle->SetDrawPhase(kPhaseForeground);
  inner_circle->SetSize(kCloseButtonWidth * 2, kCloseButtonHeight * 2);
  inner_circle->set_corner_radius(kCloseButtonWidth);
  inner_circle->set_hit_testable(false);
  BindColor(model_, inner_circle.get(),
            &ColorScheme::speech_recognition_circle_background,
            &Rect::SetColor);
  scene_->AddUiElement(kSpeechRecognitionListening, std::move(inner_circle));

  auto microphone_icon = base::MakeUnique<VectorIcon>(512);
  microphone_icon->SetIcon(vector_icons::kMicrophoneIcon);
  microphone_icon->SetName(kSpeechRecognitionListeningMicrophoneIcon);
  microphone_icon->SetDrawPhase(kPhaseForeground);
  microphone_icon->set_hit_testable(false);
  microphone_icon->SetSize(kCloseButtonWidth, kCloseButtonHeight);
  scene_->AddUiElement(kSpeechRecognitionListening, std::move(microphone_icon));

  auto close_button = Create<DiscButton>(
      kSpeechRecognitionListeningCloseButton, kPhaseForeground,
      base::BindRepeating(&UiBrowserInterface::SetVoiceSearchActive,
                          base::Unretained(browser_), false),
      vector_icons::kClose16Icon);
  close_button->SetSize(kVoiceSearchCloseButtonWidth,
                        kVoiceSearchCloseButtonHeight);
  close_button->set_hover_offset(kButtonZOffsetHoverDMM * kContentDistance);
  close_button->SetTranslate(0.0, -kVoiceSearchCloseButtonYOffset, 0.f);
  BindButtonColors(model_, close_button.get(), &ColorScheme::button_colors,
                   &DiscButton::SetButtonColors);
  scene_->AddUiElement(kSpeechRecognitionListening, std::move(close_button));

  UiElement* browser_foregroud =
      scene_->GetUiElementByName(k2dBrowsingForeground);
  browser_foregroud->AddBinding(base::MakeUnique<Binding<bool>>(
      base::Bind(
          [](UiElement* listening, UiElement* result) {
            return listening->GetTargetOpacity() == 0.f &&
                   result->GetTargetOpacity() == 0.f;
          },
          base::Unretained(listening_ui_root),
          base::Unretained(speech_result_parent)),
      base::Bind([](UiElement* e, const bool& value) { e->SetVisible(value); },
                 base::Unretained(browser_foregroud))));
}
