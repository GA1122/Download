const sync_pb::EntitySpecifics& BaseNode::GetEntitySpecifics() const {
  return GetUnencryptedSpecifics(GetEntry());
}
