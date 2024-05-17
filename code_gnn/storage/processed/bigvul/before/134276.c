void OmniboxViewViews::OnBeforePossibleChange() {
  text_before_change_ = text();
  sel_before_change_ = GetSelectedRange();
  ime_composing_before_change_ = IsIMEComposing();
}
