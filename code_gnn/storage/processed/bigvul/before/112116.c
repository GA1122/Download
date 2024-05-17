 void SyncManager::SyncInternal::SetEncryptionPassphrase(
    const std::string& passphrase,
    bool is_explicit) {
  if (passphrase.empty()) {
    NOTREACHED() << "Cannot encrypt with an empty passphrase.";
    return;
  }

  WriteTransaction trans(FROM_HERE, GetUserShare());
  Cryptographer* cryptographer = trans.GetCryptographer();
  KeyParams key_params = {"localhost", "dummy", passphrase};
  WriteNode node(&trans);
  if (node.InitByTagLookup(kNigoriTag) != sync_api::BaseNode::INIT_OK) {
    NOTREACHED();
    return;
  }

  bool nigori_has_explicit_passphrase =
      node.GetNigoriSpecifics().using_explicit_passphrase();
  std::string bootstrap_token;
  sync_pb::EncryptedData pending_keys;
  if (cryptographer->has_pending_keys())
    pending_keys = cryptographer->GetPendingKeys();
  bool success = false;


  if (!nigori_has_explicit_passphrase) {
    if (!cryptographer->has_pending_keys()) {
      if (cryptographer->AddKey(key_params)) {
        DVLOG(1) << "Setting " << (is_explicit ? "explicit" : "implicit" )
                 << " passphrase for encryption.";
        cryptographer->GetBootstrapToken(&bootstrap_token);
        success = true;
      } else {
        NOTREACHED() << "Failed to add key to cryptographer.";
        success = false;
      }
    } else {   
      if (is_explicit) {
        DVLOG(1) << "Failing because an implicit passphrase is already set.";
        success = false;
      } else {   
        if (cryptographer->DecryptPendingKeys(key_params)) {
          DVLOG(1) << "Implicit internal passphrase accepted for decryption.";
          cryptographer->GetBootstrapToken(&bootstrap_token);
          success = true;
        } else {
          DVLOG(1) << "Implicit internal passphrase failed to decrypt, adding "
                   << "anyways as default passphrase and persisting via "
                   << "bootstrap token.";
          Cryptographer temp_cryptographer(encryptor_);
          temp_cryptographer.AddKey(key_params);
          temp_cryptographer.GetBootstrapToken(&bootstrap_token);
          cryptographer->AddKey(key_params);
          success = false;
        }
      }   
    }   
  } else {   
    DVLOG(1) << "Failing because an explicit passphrase is already set.";
    success = false;
  }

  DVLOG_IF(1, !success)
      << "Failure in SetEncryptionPassphrase; notifying and returning.";
  DVLOG_IF(1, success)
      << "Successfully set encryption passphrase; updating nigori and "
         "reencrypting.";

  FinishSetPassphrase(
      success, bootstrap_token, is_explicit, &trans, &node);
}
