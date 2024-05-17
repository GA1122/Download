JsArgList SyncManager::SyncInternal::GetNodeSummariesById(
    const JsArgList& args) {
  return GetNodeInfoById(args, GetUserShare(), &BaseNode::GetSummaryAsValue);
}
