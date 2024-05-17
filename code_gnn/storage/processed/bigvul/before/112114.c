void SyncManager::SyncInternal::SetDecryptionPassphrase(
    const std::string& passphrase) {
  if (passphrase.empty()) {
    NOTREACHED() << "Cannot decrypt with an empty passphrase.";
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

  if (!cryptographer->has_pending_keys()) {
    NOTREACHED() << "Attempt to set decryption passphrase failed because there "
                 << "were no pending keys.";
    return;
  }

  bool nigori_has_explicit_passphrase =
      node.GetNigoriSpecifics().using_explicit_passphrase();
  std::string bootstrap_token;
  sync_pb::EncryptedData pending_keys;
  pending_keys = cryptographer->GetPendingKeys();
  bool success = false;

  if (!nigori_has_explicit_passphrase) {
    if (cryptographer->is_initialized()) {
      Cryptographer temp_cryptographer(encryptor_);
      temp_cryptographer.SetPendingKeys(cryptographer->GetPendingKeys());
      if (temp_cryptographer.DecryptPendingKeys(key_params)) {
        sync_pb::EncryptedData encrypted;
        cryptographer->GetKeys(&encrypted);
        if (temp_cryptographer.CanDecrypt(encrypted)) {
          DVLOG(1) << "Implicit user provided passphrase accepted for "
                   << "decryption, overwriting default.";
          cryptographer->DecryptPendingKeys(key_params);
          cryptographer->GetBootstrapToken(&bootstrap_token);
          success = true;
        } else {
          DVLOG(1) << "Implicit user provided passphrase accepted for "
                   << "decryption, restoring implicit internal passphrase "
                   << "as default.";
          std::string bootstrap_token_from_current_key;
          cryptographer->GetBootstrapToken(
              &bootstrap_token_from_current_key);
          cryptographer->DecryptPendingKeys(key_params);
          cryptographer->AddKeyFromBootstrapToken(
              bootstrap_token_from_current_key);
          success = true;
        }
      } else {   
        DVLOG(1) << "Implicit user provided passphrase failed to decrypt.";
        success = false;
      }   
    } else {   
      if (cryptographer->DecryptPendingKeys(key_params)) {
        cryptographer->GetBootstrapToken(&bootstrap_token);
        DVLOG(1) << "Implicit user provided passphrase accepted, initializing"
                 << " cryptographer.";
        success = true;
      } else {
        DVLOG(1) << "Implicit user provided passphrase failed to decrypt.";
        success = false;
      }
    }   
  } else {   
    if (cryptographer->DecryptPendingKeys(key_params)) {
      DVLOG(1) << "Explicit passphrase accepted for decryption.";
      cryptographer->GetBootstrapToken(&bootstrap_token);
      success = true;
    } else {
      DVLOG(1) << "Explicit passphrase failed to decrypt.";
      success = false;
    }
  }   

  DVLOG_IF(1, !success)
      << "Failure in SetDecryptionPassphrase; notifying and returning.";
  DVLOG_IF(1, success)
      << "Successfully set decryption passphrase; updating nigori and "
         "reencrypting.";

  FinishSetPassphrase(success,
                      bootstrap_token,
                      nigori_has_explicit_passphrase,
                      &trans,
                      &node);
}
