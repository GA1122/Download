void InlineLoginHandlerImpl::SetExtraInitParams(base::DictionaryValue& params) {
  params.SetString("service", "chromiumsync");

  Profile* profile = Profile::FromWebUI(web_ui());
  if (IsSystemProfile(profile))
    params.SetBoolean("dontResizeNonEmbeddedPages", true);

  content::WebContents* contents = web_ui()->GetWebContents();
  const GURL& current_url = contents->GetURL();
  signin_metrics::Reason reason =
      signin::GetSigninReasonForPromoURL(current_url);

    const GURL& url = GaiaUrls::GetInstance()->embedded_signin_url();
    params.SetBoolean("isNewGaiaFlow", true);
    params.SetString("clientId",
                     GaiaUrls::GetInstance()->oauth2_chrome_client_id());
    params.SetString("gaiaPath", url.path().substr(1));

    std::string flow;
    switch (reason) {
      case signin_metrics::Reason::REASON_ADD_SECONDARY_ACCOUNT:
        flow = "addaccount";
        break;
      case signin_metrics::Reason::REASON_REAUTHENTICATION:
      case signin_metrics::Reason::REASON_UNLOCK:
        flow = "reauth";
        break;
      case signin_metrics::Reason::REASON_FORCED_SIGNIN_PRIMARY_ACCOUNT:
        flow = "enterprisefsi";
        break;
      default:
        flow = "signin";
        break;
    }
    params.SetString("flow", flow);

  content::WebContentsObserver::Observe(contents);
  LogHistogramValue(signin_metrics::HISTOGRAM_SHOWN);
}
