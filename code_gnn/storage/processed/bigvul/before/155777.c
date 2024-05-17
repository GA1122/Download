bool DiceResponseHandler::CanGetTokenForAccount(const std::string& gaia_id,
                                                const std::string& email) {
  if (signin::DiceMethodGreaterOrEqual(
          account_consistency_,
          signin::AccountConsistencyMethod::kDiceMigration)) {
    return true;
  }

  DCHECK_EQ(signin::AccountConsistencyMethod::kDiceFixAuthErrors,
            account_consistency_);
  std::string account =
      account_tracker_service_->PickAccountIdForAccount(gaia_id, email);
  std::string chrome_account = signin_manager_->GetAuthenticatedAccountId();
  bool can_get_token = (chrome_account == account);
  VLOG_IF(1, !can_get_token)
      << "[Dice] Dropping Dice signin response for " << account;
  return can_get_token;
}
