void UiSceneCreator::Create2dBrowsingSubtreeRoots() {
  auto element = base::MakeUnique<UiElement>();
  element->SetName(k2dBrowsingRoot);
  element->set_hit_testable(false);
  element->AddBinding(base::MakeUnique<Binding<bool>>(
      base::BindRepeating(
          [](Model* m) {
            bool ready = !m->background_available ||
                         (m->background_available && m->background_loaded);
            return m->browsing_mode() && ready;
          },
          base::Unretained(model_)),
      base::BindRepeating([](UiElement* e, const bool& v) { e->SetVisible(v); },
                          base::Unretained(element.get()))));

  scene_->AddUiElement(kRoot, std::move(element));

  element = base::MakeUnique<UiElement>();
  element->SetName(k2dBrowsingBackground);
  element->set_hit_testable(false);
  scene_->AddUiElement(k2dBrowsingRoot, std::move(element));

  element = base::MakeUnique<UiElement>();
  element->SetName(k2dBrowsingVisibiltyControlForOmnibox);
  element->set_hit_testable(false);
  element->AddBinding(VR_BIND(bool, Model, model_, omnibox_input_active,
                              UiElement, element.get(), SetVisible(!value)));
  scene_->AddUiElement(k2dBrowsingRoot, std::move(element));

  element = base::MakeUnique<UiElement>();
  element->SetName(k2dBrowsingForeground);
  element->set_hit_testable(false);
  element->SetTransitionedProperties({OPACITY});
  element->SetTransitionDuration(base::TimeDelta::FromMilliseconds(
      kSpeechRecognitionOpacityAnimationDurationMs));
  element->AddBinding(base::MakeUnique<Binding<ModalPromptType>>(
      base::Bind([](Model* m) { return m->active_modal_prompt_type; },
                 base::Unretained(model_)),
      base::Bind(
          [](UiElement* e, const ModalPromptType& t) {
            if (t == kModalPromptTypeExitVRForSiteInfo) {
              e->SetVisibleImmediately(false);
            } else if (
                t ==
                kModalPromptTypeExitVRForVoiceSearchRecordAudioOsPermission) {
              e->SetOpacity(kModalPromptFadeOpacity);
            } else {
              e->SetVisible(true);
            }
          },
          base::Unretained(element.get()))));
  scene_->AddUiElement(k2dBrowsingVisibiltyControlForOmnibox,
                       std::move(element));

  element = base::MakeUnique<UiElement>();
  element->SetName(k2dBrowsingContentGroup);
  element->SetTranslate(0, kContentVerticalOffset, -kContentDistance);
  element->SetSize(kContentWidth, kContentHeight);
  element->set_hit_testable(false);
  element->SetTransitionedProperties({TRANSFORM});
  element->AddBinding(VR_BIND(
      bool, Model, model_, fullscreen, UiElement, element.get(),
      SetTranslate(0,
                   value ? kFullscreenVerticalOffset : kContentVerticalOffset,
                   value ? -kFullscreenToastDistance : -kContentDistance)));
  scene_->AddUiElement(k2dBrowsingForeground, std::move(element));
}
