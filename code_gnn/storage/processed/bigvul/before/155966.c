DiceTurnSyncOnHelper::DiceTurnSyncOnHelper(
    Profile* profile,
    Browser* browser,
    signin_metrics::AccessPoint signin_access_point,
    signin_metrics::PromoAction signin_promo_action,
    signin_metrics::Reason signin_reason,
    const std::string& account_id,
    SigninAbortedMode signin_aborted_mode)
    : DiceTurnSyncOnHelper(
          profile,
          signin_access_point,
          signin_promo_action,
          signin_reason,
          account_id,
          signin_aborted_mode,
          std::make_unique<DiceTurnSyncOnHelperDelegateImpl>(browser)) {}
