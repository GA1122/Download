  bool SetUpEncryption(NigoriStatus nigori_status,
                       EncryptionStatus encryption_status) {
    UserShare* share = sync_manager_.GetUserShare();
    share->directory->set_initial_sync_ended_for_type(syncable::NIGORI, true);

    int64 nigori_id = GetIdForDataType(syncable::NIGORI);
    if (nigori_id == kInvalidId)
      return false;

    WriteTransaction trans(FROM_HERE, share);
    Cryptographer* cryptographer = trans.GetCryptographer();
    if (!cryptographer)
      return false;
    if (encryption_status != UNINITIALIZED) {
      KeyParams params = {"localhost", "dummy", "foobar"};
      cryptographer->AddKey(params);
    } else {
      DCHECK_NE(nigori_status, WRITE_TO_NIGORI);
    }
    if (encryption_status == FULL_ENCRYPTION)
      cryptographer->set_encrypt_everything();
    if (nigori_status == WRITE_TO_NIGORI) {
      sync_pb::NigoriSpecifics nigori;
      cryptographer->GetKeys(nigori.mutable_encrypted());
      cryptographer->UpdateNigoriFromEncryptedTypes(&nigori);
      WriteNode node(&trans);
      EXPECT_EQ(BaseNode::INIT_OK, node.InitByIdLookup(nigori_id));
      node.SetNigoriSpecifics(nigori);
    }
    return cryptographer->is_ready();
  }
