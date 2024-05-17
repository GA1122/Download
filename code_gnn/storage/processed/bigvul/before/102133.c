JsArgList SyncManager::SyncInternal::GetNodeDetailsById(
    const JsArgList& args) {
  return GetNodeInfoById(args, GetUserShare(), &BaseNode::GetDetailsAsValue);
}
