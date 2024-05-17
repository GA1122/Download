base::string16 Textfield::GetSelectionClipboardText() const {
  base::string16 selection_clipboard_text;
  ui::Clipboard::GetForCurrentThread()->ReadText(ui::CLIPBOARD_TYPE_SELECTION,
                                                 &selection_clipboard_text);
  return selection_clipboard_text;
}
