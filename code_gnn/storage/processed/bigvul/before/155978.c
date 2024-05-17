void DiceTurnSyncOnHelper::SigninAndShowSyncConfirmationUI() {
  signin_manager_->OnExternalSigninCompleted(account_info_.email);
  signin_metrics::LogSigninAccessPointCompleted(signin_access_point_,
                                                signin_promo_action_);
  signin_metrics::LogSigninReason(signin_reason_);
  base::RecordAction(base::UserMetricsAction("Signin_Signin_Succeed"));

  browser_sync::ProfileSyncService* sync_service = GetProfileSyncService();
  if (sync_service) {
    sync_blocker_ = sync_service->GetSetupInProgressHandle();
    bool is_enterprise_user =
        !policy::BrowserPolicyConnector::IsNonEnterpriseUser(
            account_info_.email);
    if (is_enterprise_user &&
        SyncStartupTracker::GetSyncServiceState(profile_) ==
            SyncStartupTracker::SYNC_STARTUP_PENDING) {
      sync_startup_tracker_.reset(new SyncStartupTracker(profile_, this));
      return;
    }
  }

  ShowSyncConfirmationUI();
}
