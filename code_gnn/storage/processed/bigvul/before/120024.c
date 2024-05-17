void FileBrowserPrivateSearchDriveMetadataFunction::OnEntryDefinitionList(
    scoped_ptr<drive::MetadataSearchResultVector> search_result_info_list,
    scoped_ptr<EntryDefinitionList> entry_definition_list) {
  DCHECK_EQ(search_result_info_list->size(), entry_definition_list->size());
  base::ListValue* results_list = new base::ListValue();

  for (size_t i = 0; i < entry_definition_list->size(); ++i) {
    base::DictionaryValue* result_dict = new base::DictionaryValue();

    base::DictionaryValue* entry = new base::DictionaryValue();
    entry->SetString(
        "fileSystemName", entry_definition_list->at(i).file_system_name);
    entry->SetString(
        "fileSystemRoot", entry_definition_list->at(i).file_system_root_url);
    entry->SetString(
        "fileFullPath",
        "/" + entry_definition_list->at(i).full_path.AsUTF8Unsafe());
    entry->SetBoolean("fileIsDirectory",
                      entry_definition_list->at(i).is_directory);

    result_dict->Set("entry", entry);
    result_dict->SetString(
        "highlightedBaseName",
        search_result_info_list->at(i).highlighted_base_name);
    results_list->Append(result_dict);
  }

  SetResult(results_list);
  SendResponse(true);
}
