const sync_pb::PasswordSpecificsData& BaseNode::GetPasswordSpecifics() const {
  DCHECK_EQ(syncable::PASSWORDS, GetModelType());
  return *password_data_;
}
