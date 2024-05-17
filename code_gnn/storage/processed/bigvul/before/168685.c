void UiSceneCreator::CreateSplashScreenForDirectWebVrLaunch() {
  auto element = base::MakeUnique<UiElement>();
  element->SetName(kSplashScreenRoot);
  element->set_hit_testable(false);
  scene_->AddUiElement(kRoot, std::move(element));

  element = base::MakeUnique<ViewportAwareRoot>();
  element->SetName(kSplashScreenViewportAwareRoot);
  element->set_hit_testable(false);
  scene_->AddUiElement(kSplashScreenRoot, std::move(element));

  auto transient_parent = base::MakeUnique<ShowUntilSignalTransientElement>(
      base::TimeDelta::FromSeconds(kSplashScreenMinDurationSeconds),
      base::TimeDelta::Max(),
      base::Bind(
          [](Model* model, UiBrowserInterface* browser,
             TransientElementHideReason reason) {
            model->web_vr_show_splash_screen = false;
            if (reason == TransientElementHideReason::kTimeout) {
              browser->ExitPresent();
            }
          },
          base::Unretained(model_), base::Unretained(browser_)));
  transient_parent->SetName(kSplashScreenTransientParent);
  transient_parent->AddBinding(
      VR_BIND_FUNC(bool, Model, model_, web_vr_show_splash_screen, UiElement,
                   transient_parent.get(), SetVisible));
  transient_parent->set_hit_testable(false);
  transient_parent->SetTransitionedProperties({OPACITY});
  transient_parent->AddBinding(VR_BIND_FUNC(
      bool, Model, model_,
      web_vr_show_splash_screen && model->web_vr_has_produced_frames(),
      ShowUntilSignalTransientElement, transient_parent.get(), Signal));
  scene_->AddUiElement(kSplashScreenViewportAwareRoot,
                       std::move(transient_parent));

  auto text_scaler =
      base::MakeUnique<ScaledDepthAdjuster>(kSplashScreenTextDistance);
  auto text = base::MakeUnique<Text>(kSplashScreenTextFontHeightDMM);
  BindColor(model_, text.get(), &ColorScheme::splash_screen_text_color,
            &Text::SetColor);
  text->SetText(l10n_util::GetStringUTF16(IDS_VR_RUNNING_IN_CHROME_MESSAGE));
  text->SetName(kSplashScreenText);
  text->SetDrawPhase(kPhaseOverlayForeground);
  text->set_hit_testable(false);
  text->SetSize(kSplashScreenTextWidthDMM, 0);
  text->SetTranslate(0, kSplashScreenTextVerticalOffsetDMM, 0);
  text_scaler->AddChild(std::move(text));
  scene_->AddUiElement(kSplashScreenTransientParent, std::move(text_scaler));

  auto bg = base::MakeUnique<FullScreenRect>();
  bg->SetName(kSplashScreenBackground);
  bg->SetDrawPhase(kPhaseOverlayBackground);
  bg->set_hit_testable(false);
  bg->SetColor(model_->color_scheme().splash_screen_background);
  scene_->AddUiElement(kSplashScreenText, std::move(bg));

  auto spinner = base::MakeUnique<Spinner>(512);
  spinner->SetName(kWebVrTimeoutSpinner);
  spinner->SetDrawPhase(kPhaseOverlayForeground);
  spinner->SetVisible(false);
  spinner->SetSize(kSpinnerWidth, kSpinnerHeight);
  spinner->SetTranslate(0, kSpinnerVerticalOffset, -kSpinnerDistance);
  spinner->SetColor(model_->color_scheme().spinner_color);
  spinner->AddBinding(VR_BIND_FUNC(
      bool, Model, model_, web_vr_timeout_state == kWebVrTimeoutImminent,
      Spinner, spinner.get(), SetVisible));
  spinner->SetTransitionedProperties({OPACITY});
  scene_->AddUiElement(kSplashScreenViewportAwareRoot, std::move(spinner));

  auto spinner_bg = base::MakeUnique<FullScreenRect>();
  spinner_bg->SetName(kWebVrTimeoutSpinnerBackground);
  spinner_bg->SetDrawPhase(kPhaseOverlayBackground);
  spinner_bg->SetVisible(false);
  spinner_bg->set_hit_testable(false);
  spinner_bg->SetColor(model_->color_scheme().spinner_background);
  spinner_bg->SetTransitionedProperties({OPACITY});
  spinner_bg->SetTransitionDuration(base::TimeDelta::FromMilliseconds(200));
  spinner_bg->AddBinding(VR_BIND_FUNC(
      bool, Model, model_, web_vr_timeout_state != kWebVrNoTimeoutPending,
      FullScreenRect, spinner_bg.get(), SetVisible));
  scene_->AddUiElement(kSplashScreenRoot, std::move(spinner_bg));
}
