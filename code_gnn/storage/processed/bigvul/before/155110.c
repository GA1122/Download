base::string16 OmniboxViewViews::GetLabelForCommandId(int command_id) const {
  DCHECK_EQ(IDC_PASTE_AND_GO, command_id);

  constexpr size_t kMaxSelectionTextLength = 50;
  const base::string16 clipboard_text = GetClipboardText();

  base::string16 selection_text = gfx::TruncateString(
      clipboard_text, kMaxSelectionTextLength, gfx::WORD_BREAK);

  if (model()->ClassifiesAsSearch(clipboard_text))
    return l10n_util::GetStringFUTF16(IDS_PASTE_AND_SEARCH, selection_text);

  const float kMaxSelectionPixelWidth = GetStringWidthF(
      selection_text, Textfield::GetFontList(), gfx::Typesetter::BROWSER);
  base::string16 url = url_formatter::ElideUrl(
      GURL(clipboard_text), Textfield::GetFontList(), kMaxSelectionPixelWidth,
      gfx::Typesetter::BROWSER);

  return l10n_util::GetStringFUTF16(IDS_PASTE_AND_GO, url);
}
