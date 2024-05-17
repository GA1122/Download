void InlineLoginHandlerImpl::FinishCompleteLogin(
    const FinishCompleteLoginParams& params,
    Profile* profile,
    Profile::CreateStatus status) {
  std::string default_email;
  std::string validate_email;
  if (net::GetValueForKeyInQuery(params.url, "email", &default_email) &&
      net::GetValueForKeyInQuery(params.url, "validateEmail",
                                 &validate_email) &&
      validate_email == "1" && !default_email.empty()) {
    if (!gaia::AreEmailsSame(params.email, default_email)) {
      if (params.handler) {
        params.handler->HandleLoginError(
            l10n_util::GetStringFUTF8(IDS_SYNC_WRONG_EMAIL,
                                      base::UTF8ToUTF16(default_email)),
            base::UTF8ToUTF16(params.email));
      }
      return;
    }
  }

  signin_metrics::AccessPoint access_point =
      signin::GetAccessPointForPromoURL(params.url);
  signin_metrics::Reason reason =
      signin::GetSigninReasonForPromoURL(params.url);
  LogHistogramValue(signin_metrics::HISTOGRAM_ACCEPTED);
  bool switch_to_advanced =
      params.choose_what_to_sync &&
      (access_point != signin_metrics::AccessPoint::ACCESS_POINT_SETTINGS);
  LogHistogramValue(
      switch_to_advanced ? signin_metrics::HISTOGRAM_WITH_ADVANCED :
                           signin_metrics::HISTOGRAM_WITH_DEFAULTS);

  CanOfferSigninType can_offer_for = CAN_OFFER_SIGNIN_FOR_ALL_ACCOUNTS;
  switch (reason) {
    case signin_metrics::Reason::REASON_ADD_SECONDARY_ACCOUNT:
      can_offer_for = CAN_OFFER_SIGNIN_FOR_SECONDARY_ACCOUNT;
      break;
    case signin_metrics::Reason::REASON_REAUTHENTICATION:
    case signin_metrics::Reason::REASON_UNLOCK: {
      std::string primary_username =
          SigninManagerFactory::GetForProfile(profile)
              ->GetAuthenticatedAccountInfo()
              .email;
      if (!gaia::AreEmailsSame(default_email, primary_username))
        can_offer_for = CAN_OFFER_SIGNIN_FOR_SECONDARY_ACCOUNT;
      break;
    }
    default:
      break;
  }

  std::string error_msg;
  bool can_offer = CanOfferSignin(profile, can_offer_for, params.gaia_id,
                                  params.email, &error_msg);
  if (!can_offer) {
    if (params.handler) {
      params.handler->HandleLoginError(error_msg,
                                       base::UTF8ToUTF16(params.email));
    }
    return;
  }

  AboutSigninInternals* about_signin_internals =
      AboutSigninInternalsFactory::GetForProfile(profile);
  about_signin_internals->OnAuthenticationResultReceived("Successful");

  std::string signin_scoped_device_id =
      GetSigninScopedDeviceIdForProfile(profile);
  base::WeakPtr<InlineLoginHandlerImpl> handler_weak_ptr;
  if (params.handler)
    handler_weak_ptr = params.handler->GetWeakPtr();

  new InlineSigninHelper(
      handler_weak_ptr,
      params.partition->GetURLLoaderFactoryForBrowserProcess(), profile, status,
      params.url, params.email, params.gaia_id, params.password,
      params.auth_code, signin_scoped_device_id, params.choose_what_to_sync,
      params.confirm_untrusted_signin,
      params.is_force_sign_in_with_usermanager);

  if (!params.is_force_sign_in_with_usermanager) {
    UnlockProfileAndHideLoginUI(params.profile_path, params.handler);
  }
}
