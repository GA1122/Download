void DiceTurnSyncOnHelper::OnRegisteredForPolicy(const std::string& dm_token,
                                                 const std::string& client_id) {
  if (dm_token.empty()) {
    DVLOG(1) << "Policy registration failed";
    SigninAndShowSyncConfirmationUI();
    return;
  }

  DVLOG(1) << "Policy registration succeeded: dm_token=" << dm_token;

  DCHECK(dm_token_.empty());
  DCHECK(client_id_.empty());
  dm_token_ = dm_token;
  client_id_ = client_id;

  delegate_->ShowEnterpriseAccountConfirmation(
      account_info_.email,
      base::BindOnce(&DiceTurnSyncOnHelper::OnEnterpriseAccountConfirmation,
                     weak_pointer_factory_.GetWeakPtr()));
}
