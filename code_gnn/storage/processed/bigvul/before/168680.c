void UiSceneCreator::CreateExitPrompt() {
  std::unique_ptr<UiElement> element;

  auto backplane = base::MakeUnique<InvisibleHitTarget>();
  backplane->SetName(kExitPromptBackplane);
  backplane->SetDrawPhase(kPhaseForeground);
  backplane->SetSize(kPromptBackplaneSize, kPromptBackplaneSize);
  backplane->SetTranslate(0.0,
                          kContentVerticalOffset + kExitPromptVerticalOffset,
                          -kContentDistance);
  EventHandlers event_handlers;
  event_handlers.button_up = base::Bind(
      [](UiBrowserInterface* browser, Model* m) {
        browser->OnExitVrPromptResult(
            ExitVrPromptChoice::CHOICE_NONE,
            GetReasonForPrompt(m->active_modal_prompt_type));
      },
      base::Unretained(browser_), base::Unretained(model_));
  backplane->set_event_handlers(event_handlers);
  backplane->AddBinding(VR_BIND_FUNC(
      bool, Model, model_,
      active_modal_prompt_type == kModalPromptTypeExitVRForSiteInfo, UiElement,
      backplane.get(), SetVisible));
  scene_->AddUiElement(k2dBrowsingRoot, std::move(backplane));

  std::unique_ptr<ExitPrompt> exit_prompt = base::MakeUnique<ExitPrompt>(
      512,
      base::Bind(&UiBrowserInterface::OnExitVrPromptResult,
                 base::Unretained(browser_), ExitVrPromptChoice::CHOICE_STAY),
      base::Bind(&UiBrowserInterface::OnExitVrPromptResult,
                 base::Unretained(browser_), ExitVrPromptChoice::CHOICE_EXIT));
  exit_prompt->SetName(kExitPrompt);
  exit_prompt->SetDrawPhase(kPhaseForeground);
  exit_prompt->SetSize(kExitPromptWidth, kExitPromptHeight);
  BindColor(model_, exit_prompt.get(), &ColorScheme::prompt_foreground,
            &TexturedElement::SetForegroundColor);
  BindButtonColors(model_, exit_prompt.get(),
                   &ColorScheme::prompt_primary_button_colors,
                   &ExitPrompt::SetPrimaryButtonColors);
  BindButtonColors(model_, exit_prompt.get(),
                   &ColorScheme::prompt_secondary_button_colors,
                   &ExitPrompt::SetSecondaryButtonColors);
  exit_prompt->AddBinding(base::MakeUnique<Binding<ModalPromptType>>(
      base::Bind([](Model* m) { return m->active_modal_prompt_type; },
                 base::Unretained(model_)),
      base::Bind(
          [](ExitPrompt* e, const ModalPromptType& p) {
            e->set_reason(GetReasonForPrompt(p));
            switch (p) {
              case kModalPromptTypeExitVRForSiteInfo:
                e->SetContentMessageId(
                    IDS_VR_SHELL_EXIT_PROMPT_DESCRIPTION_SITE_INFO);
                break;
              default:
                e->SetContentMessageId(IDS_VR_SHELL_EXIT_PROMPT_DESCRIPTION);
                break;
            }
          },
          base::Unretained(exit_prompt.get()))));
  scene_->AddUiElement(kExitPromptBackplane, std::move(exit_prompt));
}
