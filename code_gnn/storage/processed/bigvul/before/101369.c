const sync_pb::BookmarkSpecifics& BaseNode::GetBookmarkSpecifics() const {
  DCHECK_EQ(syncable::BOOKMARKS, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::bookmark);
}
