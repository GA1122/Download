const sync_pb::NigoriSpecifics& BaseNode::GetNigoriSpecifics() const {
  DCHECK_EQ(syncable::NIGORI, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::nigori);
}
