  bool SetUpEncryption() {
    #if defined(OS_MACOSX)
      Encryptor::UseMockKeychain(true);
    #endif

    UserShare* share = sync_manager_.GetUserShare();
    int64 nigori_id = GetIdForDataType(syncable::NIGORI);
    if (nigori_id == kInvalidId)
      return false;

    WriteTransaction trans(FROM_HERE, share);
    Cryptographer* cryptographer = trans.GetCryptographer();
    if (!cryptographer)
      return false;
    KeyParams params = {"localhost", "dummy", "foobar"};
    cryptographer->AddKey(params);
    sync_pb::NigoriSpecifics nigori;
    cryptographer->GetKeys(nigori.mutable_encrypted());
    WriteNode node(&trans);
    EXPECT_TRUE(node.InitByIdLookup(nigori_id));
    node.SetNigoriSpecifics(nigori);
    return cryptographer->is_ready();
  }
