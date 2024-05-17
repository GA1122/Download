void DiceTurnSyncOnHelper::OnPolicyFetchComplete(bool success) {
  DLOG_IF(ERROR, !success) << "Error fetching policy for user";
  DVLOG_IF(1, success) << "Policy fetch successful - completing signin";
  SigninAndShowSyncConfirmationUI();
}
