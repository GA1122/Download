 bool WebviewInfo::IsResourceWebviewAccessible(
     const Extension* extension,
     const std::string& partition_id,
     const std::string& relative_path) {
   if (!extension)
     return false;
 
   const WebviewInfo* info = GetResourcesInfo(*extension);
    if (!info)
      return false;
  
  bool partition_is_privileged = false;
  for (size_t i = 0;
       i < info->webview_privileged_partitions_.size();
       ++i) {
    if (MatchPattern(partition_id, info->webview_privileged_partitions_[i])) {
      partition_is_privileged = true;
      break;
//   for (size_t i = 0; i < info->partition_items_.size(); ++i) {
//     const PartitionItem* const item = info->partition_items_[i];
//     if (item->Matches(partition_id) &&
//         extension->ResourceMatches(item->accessible_resources(),
//                                    relative_path)) {
//       return true;
      }
    }
  
  return partition_is_privileged && extension->ResourceMatches(
      info->webview_accessible_resources_, relative_path);
//   return false;
// }
// 
// void WebviewInfo::AddPartitionItem(scoped_ptr<PartitionItem> item) {
//   partition_items_.push_back(item.release());
  }