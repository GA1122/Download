std::vector<DropData::Metadata> DropDataToMetaData(const DropData& drop_data) {
  std::vector<DropData::Metadata> metadata;
  if (!drop_data.text.is_null()) {
    metadata.push_back(DropData::Metadata::CreateForMimeType(
        DropData::Kind::STRING,
        base::ASCIIToUTF16(ui::Clipboard::kMimeTypeText)));
  }

  if (drop_data.url.is_valid()) {
    metadata.push_back(DropData::Metadata::CreateForMimeType(
        DropData::Kind::STRING,
        base::ASCIIToUTF16(ui::Clipboard::kMimeTypeURIList)));
  }

  if (!drop_data.html.is_null()) {
    metadata.push_back(DropData::Metadata::CreateForMimeType(
        DropData::Kind::STRING,
        base::ASCIIToUTF16(ui::Clipboard::kMimeTypeHTML)));
  }

  for (const auto& file_info : drop_data.filenames) {
    if (!file_info.path.empty()) {
      metadata.push_back(DropData::Metadata::CreateForFilePath(file_info.path));
    }
  }

  for (const auto& mime_type : drop_data.file_mime_types) {
    if (!mime_type.empty()) {
      metadata.push_back(DropData::Metadata::CreateForMimeType(
          DropData::Kind::FILENAME, mime_type));
    }
  }

  for (const auto& file_system_file : drop_data.file_system_files) {
    if (!file_system_file.url.is_empty()) {
      metadata.push_back(
          DropData::Metadata::CreateForFileSystemUrl(file_system_file.url));
    }
  }

  for (const auto& custom_data_item : drop_data.custom_data) {
    metadata.push_back(DropData::Metadata::CreateForMimeType(
        DropData::Kind::STRING, custom_data_item.first));
  }

  return metadata;
}
