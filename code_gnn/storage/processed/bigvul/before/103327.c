void ClipboardMessageFilter::OnReadFilenames(
    ui::Clipboard::Buffer buffer, bool* succeeded,
    std::vector<string16>* filenames) {
  *succeeded = ClipboardDispatcher::ReadFilenames(buffer, filenames);
}
