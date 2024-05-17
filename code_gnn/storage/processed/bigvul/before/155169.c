void OmniboxViewViews::SetUserText(const base::string16& text,
                                   bool update_popup) {
  saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
  OmniboxView::SetUserText(text, update_popup);
}
