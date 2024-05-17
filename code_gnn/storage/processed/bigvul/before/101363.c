bool BaseNode::DecryptIfNecessary() {
  if (!GetEntry()->Get(syncable::UNIQUE_SERVER_TAG).empty())
      return true;   
  const sync_pb::EntitySpecifics& specifics =
      GetEntry()->Get(syncable::SPECIFICS);
  if (specifics.HasExtension(sync_pb::password)) {
    scoped_ptr<sync_pb::PasswordSpecificsData> data(DecryptPasswordSpecifics(
        specifics, GetTransaction()->GetCryptographer()));
    if (!data.get()) {
      LOG(ERROR) << "Failed to decrypt password specifics.";
      return false;
    }
    password_data_.swap(data);
    return true;
  }

  if (!specifics.has_encrypted()) {
    if (GetModelType() == syncable::BOOKMARKS &&
        !specifics.GetExtension(sync_pb::bookmark).has_title() &&
        !GetTitle().empty()) {   
      std::string title = GetTitle();
      std::string server_legal_title;
      SyncAPINameToServerName(title, &server_legal_title);
      VLOG(1) << "Reading from legacy bookmark, manually returning title "
              << title;
      unencrypted_data_.CopyFrom(specifics);
      unencrypted_data_.MutableExtension(sync_pb::bookmark)->set_title(
          server_legal_title);
    }
    return true;
  }

  const sync_pb::EncryptedData& encrypted = specifics.encrypted();
  std::string plaintext_data = GetTransaction()->GetCryptographer()->
      DecryptToString(encrypted);
  if (plaintext_data.length() == 0 ||
      !unencrypted_data_.ParseFromString(plaintext_data)) {
    LOG(ERROR) << "Failed to decrypt encrypted node of type " <<
      syncable::ModelTypeToString(GetModelType()) << ".";
    return false;
  }
  VLOG(2) << "Decrypted specifics of type "
          << syncable::ModelTypeToString(GetModelType())
          << " with content: " << plaintext_data;
  return true;
}
