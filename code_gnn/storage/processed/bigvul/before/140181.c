void GaiaCookieManagerService::Init() {
  cookie_changed_subscription_ = signin_client_->AddCookieChangedCallback(
      GaiaUrls::GetInstance()->google_url(), kGaiaCookieName,
      base::Bind(&GaiaCookieManagerService::OnCookieChanged,
                 base::Unretained(this)));
}
