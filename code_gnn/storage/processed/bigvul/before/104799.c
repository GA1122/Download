std::vector<ExtensionSyncData> ExtensionService::GetSyncDataList(
    ExtensionFilter filter) const {
  std::vector<ExtensionSyncData> sync_data_list;
  GetSyncDataListHelper(extensions_, filter, &sync_data_list);
  GetSyncDataListHelper(disabled_extensions_, filter, &sync_data_list);
  GetSyncDataListHelper(terminated_extensions_, filter, &sync_data_list);
  return sync_data_list;
}
