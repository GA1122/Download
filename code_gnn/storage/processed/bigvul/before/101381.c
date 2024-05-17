const sync_pb::SessionSpecifics& BaseNode::GetSessionSpecifics() const {
  DCHECK_EQ(syncable::SESSIONS, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::session);
}
