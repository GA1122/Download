void ClipboardMessageFilter::OnReadAvailableTypes(
    ui::Clipboard::Buffer buffer, std::vector<string16>* types,
    bool* contains_filenames) {
  GetClipboard()->ReadAvailableTypes(buffer, types, contains_filenames);
}
