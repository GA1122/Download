  ui::ClipboardType GetAndResetCopiedToClipboard() {
    ui::ClipboardType clipboard_type = copied_to_clipboard_;
    copied_to_clipboard_ = ui::CLIPBOARD_TYPE_LAST;
    return clipboard_type;
  }
