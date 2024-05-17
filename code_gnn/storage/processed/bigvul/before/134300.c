void OmniboxViewViews::SetUserText(const base::string16& text,
                                   const base::string16& display_text,
                                   bool update_popup) {
  saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
  OmniboxView::SetUserText(text, display_text, update_popup);
}
