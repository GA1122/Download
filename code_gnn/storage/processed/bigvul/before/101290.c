UpdateAttemptResponse SyncerUtil::AttemptToUpdateEntry(
    syncable::WriteTransaction* const trans,
    syncable::MutableEntry* const entry,
    ConflictResolver* resolver,
    Cryptographer* cryptographer) {
  CHECK(entry->good());
  if (!entry->Get(IS_UNAPPLIED_UPDATE))
    return SUCCESS;   
  syncable::Id id = entry->Get(ID);

  if (entry->Get(IS_UNSYNCED)) {
    VLOG(1) << "Skipping update, returning conflict for: " << id
            << " ; it's unsynced.";
    return CONFLICT;
  }
  if (!entry->Get(SERVER_IS_DEL)) {
    syncable::Id new_parent = entry->Get(SERVER_PARENT_ID);
    Entry parent(trans, GET_BY_ID,  new_parent);
    if (!parent.good() || parent.Get(IS_DEL) || !parent.Get(IS_DIR)) {
      return CONFLICT;
    }
    if (entry->Get(PARENT_ID) != new_parent) {
      if (!entry->Get(IS_DEL) && !IsLegalNewParent(trans, id, new_parent)) {
        VLOG(1) << "Not updating item " << id
                << ", illegal new parent (would cause loop).";
        return CONFLICT;
      }
    }
  } else if (entry->Get(IS_DIR)) {
    Directory::ChildHandles handles;
    trans->directory()->GetChildHandlesById(trans, id, &handles);
    if (!handles.empty()) {
      VLOG(1) << "Not deleting directory; it's not empty " << *entry;
      return CONFLICT;
    }
  }

  const sync_pb::EntitySpecifics& specifics = entry->Get(SERVER_SPECIFICS);
  if (specifics.HasExtension(sync_pb::nigori)) {
    const sync_pb::NigoriSpecifics& nigori =
        specifics.GetExtension(sync_pb::nigori);
    cryptographer->Update(nigori);

    syncable::ModelTypeSet encrypted_types =
        cryptographer->GetEncryptedTypes();
    if (!VerifyUnsyncedChangesAreEncrypted(trans, encrypted_types) &&
        (!cryptographer->is_ready() ||
         !syncable::ProcessUnsyncedChangesForEncryption(trans,
                                                        cryptographer))) {
      VLOG(1) << "Marking nigori node update as conflicting due to being unable"
              << " to encrypt all necessary unsynced changes.";
      return CONFLICT_ENCRYPTION;
    }

  }

  if (specifics.has_encrypted() &&
      !cryptographer->CanDecrypt(specifics.encrypted())) {
    VLOG(1) << "Received an undecryptable "
            << syncable::ModelTypeToString(entry->GetServerModelType())
            << " update, returning encryption_conflict.";
    return CONFLICT_ENCRYPTION;
  } else if (specifics.HasExtension(sync_pb::password) &&
             entry->Get(UNIQUE_SERVER_TAG).empty()) {
    const sync_pb::PasswordSpecifics& password =
        specifics.GetExtension(sync_pb::password);
    if (!cryptographer->CanDecrypt(password.encrypted())) {
      VLOG(1) << "Received an undecryptable password update, returning "
              << "encryption_conflict.";
      return CONFLICT_ENCRYPTION;
    }
  } else {
    if (specifics.has_encrypted()) {
      VLOG(2) << "Received a decryptable "
              << syncable::ModelTypeToString(entry->GetServerModelType())
              << " update, applying normally.";
    } else {
      VLOG(2) << "Received an unencrypted "
              << syncable::ModelTypeToString(entry->GetServerModelType())
              << " update, applying normally.";
    }
  }

  SyncerUtil::UpdateLocalDataFromServerData(trans, entry);

  return SUCCESS;
}
