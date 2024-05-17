BookmarkNodeData::GetBookmarkCustomFormat() {
  CR_DEFINE_STATIC_LOCAL(
      ui::OSExchangeData::CustomFormat,
      format,
      (ui::Clipboard::GetFormatType(BookmarkNodeData::kClipboardFormatString)));

  return format;
}
