void ConvertSearchResultInfoListToEntryDefinitionList(
    Profile* profile,
    const std::string& extension_id,
    const std::vector<T>& search_result_info_list,
    const EntryDefinitionListCallback& callback) {
  FileDefinitionList file_definition_list;

  for (size_t i = 0; i < search_result_info_list.size(); ++i) {
    FileDefinition file_definition;
    file_definition.virtual_path =
        file_manager::util::ConvertDrivePathToRelativeFileSystemPath(
            profile, extension_id, search_result_info_list.at(i).path);
    file_definition.is_directory = search_result_info_list.at(i).is_directory;
    file_definition_list.push_back(file_definition);
  }

  file_manager::util::ConvertFileDefinitionListToEntryDefinitionList(
      profile,
      extension_id,
      file_definition_list,   
      callback);
}
