void OmniboxViewViews::Init() {
  SetController(this);
  SetTextInputType(DetermineTextInputType());
  SetBackgroundColor(location_bar_view_->GetColor(
      ToolbarModel::NONE, LocationBarView::BACKGROUND));

  if (popup_window_mode_)
    SetReadOnly(true);

  if (chrome::ShouldDisplayOriginChip())
    set_placeholder_text(l10n_util::GetStringUTF16(IDS_OMNIBOX_EMPTY_HINT));

  popup_view_.reset(OmniboxPopupContentsView::Create(
      GetFontList(), this, model(), location_bar_view_));

#if defined(OS_CHROMEOS)
  chromeos::input_method::InputMethodManager::Get()->
      AddCandidateWindowObserver(this);
#endif
}
