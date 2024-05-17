bool BaseNode::GetIsFolder() const {
  return GetEntry()->Get(syncable::IS_DIR);
}
