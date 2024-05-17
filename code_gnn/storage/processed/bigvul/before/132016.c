void ClipboardMessageFilter::OnReadAvailableTypes(
    ui::ClipboardType type,
    std::vector<base::string16>* types,
    bool* contains_filenames) {
  GetClipboard()->ReadAvailableTypes(type, types, contains_filenames);
}
