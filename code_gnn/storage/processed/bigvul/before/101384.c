std::string BaseNode::GetTitle() const {
  std::string result;
  if (syncable::BOOKMARKS == GetModelType() &&
      GetEntry()->Get(syncable::SPECIFICS).has_encrypted()) {
    ServerNameToSyncAPIName(GetBookmarkSpecifics().title(), &result);
  } else {
    ServerNameToSyncAPIName(GetEntry()->Get(syncable::NON_UNIQUE_NAME),
                            &result);
  }
  return result;
}
