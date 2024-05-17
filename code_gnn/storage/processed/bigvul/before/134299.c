void OmniboxViewViews::SetTextAndSelectedRange(const base::string16& text,
                                               const gfx::Range& range) {
  SetText(text);
  SelectRange(range);
}
