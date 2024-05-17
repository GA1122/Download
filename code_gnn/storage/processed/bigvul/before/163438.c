void OmniboxViewViews::Init() {
  set_controller(this);
  SetTextInputType(ui::TEXT_INPUT_TYPE_URL);
  GetRenderText()->SetElideBehavior(gfx::ELIDE_TAIL);

  if (popup_window_mode_)
    SetReadOnly(true);

  if (location_bar_view_) {
    popup_view_.reset(new OmniboxPopupContentsView(GetFontList(), this, model(),
                                                   location_bar_view_));
  }

  SetBorder(views::CreateEmptyBorder(
      ChromeLayoutProvider::Get()->GetInsetsMetric(INSETS_OMNIBOX)));

#if defined(OS_CHROMEOS)
  chromeos::input_method::InputMethodManager::Get()->
      AddCandidateWindowObserver(this);
#endif
}
