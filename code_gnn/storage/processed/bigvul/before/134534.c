void PrepareDragForFileContents(const DropData& drop_data,
                                ui::OSExchangeData::Provider* provider) {
  base::FilePath file_name(drop_data.file_description_filename);
  if (file_name.BaseName().RemoveExtension().empty()) {
    const base::string16 extension = file_name.Extension();
    file_name = base::FilePath(net::GetSuggestedFilename(
        drop_data.url, "", "", "", "", "")).ReplaceExtension(extension);
  }
  provider->SetFileContents(file_name, drop_data.file_contents);
}
