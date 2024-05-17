void FileBrowserPrivateSearchDriveFunction::OnEntryDefinitionList(
    const GURL& next_link,
    scoped_ptr<SearchResultInfoList> search_result_info_list,
    scoped_ptr<EntryDefinitionList> entry_definition_list) {
  DCHECK_EQ(search_result_info_list->size(), entry_definition_list->size());
  base::ListValue* entries = new base::ListValue();

  for (EntryDefinitionList::const_iterator it = entry_definition_list->begin();
       it != entry_definition_list->end();
       ++it) {
    base::DictionaryValue* entry = new base::DictionaryValue();
    entry->SetString("fileSystemName", it->file_system_name);
    entry->SetString("fileSystemRoot", it->file_system_root_url);
    entry->SetString("fileFullPath", "/" + it->full_path.AsUTF8Unsafe());
    entry->SetBoolean("fileIsDirectory", it->is_directory);
    entries->Append(entry);
  }

  base::DictionaryValue* result = new base::DictionaryValue();
  result->Set("entries", entries);
  result->SetString("nextFeed", next_link.spec());

  SetResult(result);
  SendResponse(true);
}
