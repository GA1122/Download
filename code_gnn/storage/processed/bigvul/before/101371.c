const sync_pb::ExtensionSpecifics& BaseNode::GetExtensionSpecifics() const {
  DCHECK_EQ(syncable::EXTENSIONS, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::extension);
}
