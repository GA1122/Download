std::unique_ptr<Rect> CreateOmniboxSpacer(Model* model) {
  auto spacer = Create<Rect>(kNone, kPhaseForeground);
  spacer->SetType(kTypeOmniboxSuggestionSpacer);
  spacer->SetSize(kOmniboxWidthDMM, kSuggestionVerticalPaddingDMM);
  spacer->AddBinding(base::MakeUnique<Binding<bool>>(
      base::BindRepeating(
          [](Model* m) { return !m->omnibox_suggestions.empty(); },
          base::Unretained(model)),
      base::BindRepeating(
          [](UiElement* e, const bool& v) {
            e->SetVisible(v);
            e->set_requires_layout(v);
          },
          base::Unretained(spacer.get()))));
  BindColor(model, spacer.get(), &ColorScheme::omnibox_background,
            &Rect::SetColor);
  return spacer;
}
