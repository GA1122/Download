const sync_pb::AppSpecifics& BaseNode::GetAppSpecifics() const {
  DCHECK_EQ(syncable::APPS, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::app);
}
