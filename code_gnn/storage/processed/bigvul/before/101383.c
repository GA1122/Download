const sync_pb::ThemeSpecifics& BaseNode::GetThemeSpecifics() const {
  DCHECK_EQ(syncable::THEMES, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::theme);
}
