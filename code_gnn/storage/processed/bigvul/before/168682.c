void UiSceneCreator::CreateOmnibox() {
  auto scaler = base::MakeUnique<ScaledDepthAdjuster>(kUrlBarDistance);
  scaler->SetName(kOmniboxDmmRoot);

  auto omnibox_root = base::MakeUnique<UiElement>();
  omnibox_root->SetName(kOmniboxRoot);
  omnibox_root->SetDrawPhase(kPhaseNone);
  omnibox_root->SetVisible(false);
  omnibox_root->set_hit_testable(false);
  omnibox_root->SetTransitionedProperties({OPACITY});
  omnibox_root->AddBinding(VR_BIND_FUNC(bool, Model, model_,
                                        omnibox_input_active, UiElement,
                                        omnibox_root.get(), SetVisible));

  auto shadow = base::MakeUnique<Shadow>();
  shadow->SetName(kOmniboxShadow);
  shadow->SetDrawPhase(kPhaseForeground);
  shadow->set_intensity(kOmniboxShadowIntensity);
  shadow->set_y_anchoring(TOP);
  shadow->set_y_centering(BOTTOM);
  shadow->set_corner_radius(kOmniboxCornerRadiusDMM);

  auto omnibox_outer_layout = base::MakeUnique<LinearLayout>(LinearLayout::kUp);
  omnibox_outer_layout->set_hit_testable(false);
  omnibox_outer_layout->SetName(kOmniboxOuterLayout);
  omnibox_outer_layout->set_margin(kSuggestionGapDMM);
  omnibox_outer_layout->SetTranslate(
      0, kUrlBarVerticalOffsetDMM - 0.5 * kOmniboxHeightDMM,
      kOmniboxShadowOffset);
  omnibox_outer_layout->AddBinding(base::MakeUnique<Binding<bool>>(
      base::BindRepeating([](Model* m) { return m->omnibox_input_active; },
                          base::Unretained(model_)),
      base::BindRepeating(
          [](UiElement* e, const bool& v) {
            float y_offset =
                v ? kOmniboxVerticalOffsetDMM : kUrlBarVerticalOffsetDMM;
            y_offset -= 0.5 * kOmniboxHeightDMM;
            e->SetTranslate(0, y_offset, kOmniboxShadowOffset);
          },
          omnibox_outer_layout.get())));

  auto omnibox_container = base::MakeUnique<Rect>();
  omnibox_container->SetName(kOmniboxContainer);
  omnibox_container->SetDrawPhase(kPhaseForeground);
  omnibox_container->SetSize(kOmniboxWidthDMM, kOmniboxHeightDMM);
  omnibox_container->SetTransitionedProperties({TRANSFORM, OPACITY});
  omnibox_container->SetTransitionDuration(
      base::TimeDelta::FromMilliseconds(kOmniboxTransitionMs));
  omnibox_container->set_focusable(false);
  omnibox_container->AddBinding(base::MakeUnique<Binding<bool>>(
      base::BindRepeating(
          [](Model* m) { return m->omnibox_suggestions.empty(); },
          base::Unretained(model_)),
      base::BindRepeating(
          [](Rect* r, const bool& v) {
            if (v) {
              r->set_corner_radii(
                  {kOmniboxCornerRadiusDMM, kOmniboxCornerRadiusDMM,
                   kOmniboxCornerRadiusDMM, kOmniboxCornerRadiusDMM});
            } else {
              r->set_corner_radii(
                  {0, 0, kOmniboxCornerRadiusDMM, kOmniboxCornerRadiusDMM});
            }
          },
          omnibox_container.get())));
  BindColor(model_, omnibox_container.get(), &ColorScheme::omnibox_background,
            &Rect::SetColor);

  float width = kOmniboxWidthDMM - 2 * kOmniboxTextMarginDMM;
  auto omnibox_text_field =
      CreateTextInput(kOmniboxTextHeightDMM, model_,
                      &model_->omnibox_text_field_info, text_input_delegate_);
  omnibox_text_field->set_input_committed_callback(base::BindRepeating(
      [](Model* model, UiBrowserInterface* browser, const TextInputInfo& text) {
        if (!model->omnibox_suggestions.empty()) {
          browser->Navigate(model->omnibox_suggestions.front().destination);
          model->omnibox_input_active = false;
        }
      },
      base::Unretained(model_), base::Unretained(browser_)));
  omnibox_text_field->AddBinding(
      VR_BIND(TextInputInfo, Model, model_, omnibox_text_field_info,
              UiBrowserInterface, browser_, StartAutocomplete(value.text)));
  omnibox_text_field->SetSize(width, 0);
  omnibox_text_field->SetHintText(
      l10n_util::GetStringUTF16(IDS_SEARCH_OR_TYPE_URL));
  omnibox_text_field->SetName(kOmniboxTextField);
  omnibox_text_field->set_x_anchoring(LEFT);
  omnibox_text_field->set_x_centering(LEFT);
  omnibox_text_field->SetTranslate(kOmniboxTextMarginDMM, 0, 0);
  omnibox_text_field->AddBinding(base::MakeUnique<Binding<bool>>(
      base::BindRepeating([](Model* m) { return m->omnibox_input_active; },
                          base::Unretained(model_)),
      base::BindRepeating(
          [](TextInput* e, Model* m, const bool& v) {
            m->omnibox_text_field_info = TextInputInfo();
            if (v) {
              e->RequestFocus();
            } else {
              e->RequestUnfocus();
            }
          },
          base::Unretained(omnibox_text_field.get()),
          base::Unretained(model_))));
  omnibox_text_field->AddBinding(base::MakeUnique<Binding<AutocompleteStatus>>(
      base::BindRepeating(
          [](Model* m) {
            AutocompleteStatus state;
            state.active = m->omnibox_input_active;
            state.input = m->omnibox_text_field_info.text;
            return state;
          },
          base::Unretained(model_)),
      base::BindRepeating(
          [](UiBrowserInterface* browser, const AutocompleteStatus& r) {
            if (r.active) {
              browser->StartAutocomplete(r.input);
            } else {
              browser->StopAutocomplete();
            }
          },
          base::Unretained(browser_))));
  BindColor(model_, omnibox_text_field.get(), &ColorScheme::omnibox_text,
            &TextInput::SetTextColor);
  BindColor(model_, omnibox_text_field.get(), &ColorScheme::cursor,
            &TextInput::SetCursorColor);
  BindColor(model_, omnibox_text_field.get(), &ColorScheme::omnibox_hint,
            &TextInput::SetHintColor);

  SuggestionSetBinding::ModelAddedCallback added_callback =
      base::Bind(&OnSuggestionModelAdded, base::Unretained(scene_),
                 base::Unretained(browser_), base::Unretained(model_));
  SuggestionSetBinding::ModelRemovedCallback removed_callback =
      base::Bind(&OnSuggestionModelRemoved, base::Unretained(scene_));

  auto suggestions_outer_layout =
      base::MakeUnique<LinearLayout>(LinearLayout::kDown);
  suggestions_outer_layout->SetName(kOmniboxSuggestionsOuterLayout);
  suggestions_outer_layout->set_hit_testable(false);

  auto suggestions_layout = base::MakeUnique<LinearLayout>(LinearLayout::kUp);
  suggestions_layout->SetName(kOmniboxSuggestions);
  suggestions_layout->SetDrawPhase(kPhaseNone);
  suggestions_layout->set_hit_testable(false);
  suggestions_layout->AddBinding(base::MakeUnique<SuggestionSetBinding>(
      &model_->omnibox_suggestions, added_callback, removed_callback));

  auto button_scaler =
      base::MakeUnique<ScaledDepthAdjuster>(kOmniboxCloseButtonDepthOffset);

  auto close_button = Create<DiscButton>(
      kOmniboxCloseButton, kPhaseForeground,
      base::BindRepeating([](Model* m) { m->omnibox_input_active = false; },
                          base::Unretained(model_)),
      vector_icons::kBackArrowIcon);
  close_button->SetSize(kOmniboxCloseButtonDiameterDMM,
                        kOmniboxCloseButtonDiameterDMM);
  close_button->SetTranslate(0, kOmniboxCloseButtonVerticalOffsetDMM, 0);
  close_button->SetRotate(1, 0, 0, atan(kOmniboxCloseButtonVerticalOffsetDMM));
  close_button->set_hover_offset(kButtonZOffsetHoverDMM);
  BindButtonColors(model_, close_button.get(), &ColorScheme::button_colors,
                   &DiscButton::SetButtonColors);

  auto spacer = CreateOmniboxSpacer(model_);
  spacer->set_corner_radii(
      {kOmniboxCornerRadiusDMM, kOmniboxCornerRadiusDMM, 0, 0});
  suggestions_outer_layout->AddChild(std::move(spacer));
  suggestions_outer_layout->AddChild(std::move(suggestions_layout));
  suggestions_outer_layout->AddChild(CreateOmniboxSpacer(model_));

  omnibox_container->AddChild(std::move(omnibox_text_field));

  omnibox_outer_layout->AddChild(std::move(omnibox_container));
  omnibox_outer_layout->AddChild(std::move(suggestions_outer_layout));

  shadow->AddChild(std::move(omnibox_outer_layout));

  button_scaler->AddChild(std::move(close_button));

  omnibox_root->AddChild(std::move(shadow));
  omnibox_root->AddChild(std::move(button_scaler));

  scaler->AddChild(std::move(omnibox_root));

  scene_->AddUiElement(k2dBrowsingRoot, std::move(scaler));
}
