  void PrintPreviewDataService::SetDataEntry(
    const std::string& preview_ui_addr_str,
//     int32 preview_ui_id,
      int index,
      const base::RefCountedBytes* data_bytes) {
  PreviewDataStoreMap::iterator it = data_store_map_.find(preview_ui_addr_str);
  if (it == data_store_map_.end())
    data_store_map_[preview_ui_addr_str] = new PrintPreviewDataStore();
//   if (!ContainsKey(data_store_map_, preview_ui_id))
//     data_store_map_[preview_ui_id] = new PrintPreviewDataStore();
  
  data_store_map_[preview_ui_addr_str]->SetPreviewDataForIndex(index,
                                                               data_bytes);
//   data_store_map_[preview_ui_id]->SetPreviewDataForIndex(index, data_bytes);
  }