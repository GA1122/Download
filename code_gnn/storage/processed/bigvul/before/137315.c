void Textfield::UpdateSelectionClipboard() {
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  if (text_input_type_ != ui::TEXT_INPUT_TYPE_PASSWORD) {
    ui::ScopedClipboardWriter(ui::CLIPBOARD_TYPE_SELECTION)
        .WriteText(GetSelectedText());
    if (controller_)
      controller_->OnAfterCutOrCopy(ui::CLIPBOARD_TYPE_SELECTION);
  }
#endif
}
