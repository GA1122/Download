const sync_pb::TypedUrlSpecifics& BaseNode::GetTypedUrlSpecifics() const {
  DCHECK_EQ(syncable::TYPED_URLS, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::typed_url);
}
