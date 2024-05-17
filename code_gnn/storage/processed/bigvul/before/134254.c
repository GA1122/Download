base::string16 OmniboxViewViews::GetLabelForCommandId(int command_id) const {
  DCHECK_EQ(IDS_PASTE_AND_GO, command_id);
  return l10n_util::GetStringUTF16(
      model()->IsPasteAndSearch(GetClipboardText()) ?
          IDS_PASTE_AND_SEARCH : IDS_PASTE_AND_GO);
}
