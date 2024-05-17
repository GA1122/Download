void SafeBrowsingPrivateEventRouter::OnPolicySpecifiedPasswordReuseDetected(
    const GURL& url,
    const std::string& user_name,
    bool is_phishing_url) {
  api::safe_browsing_private::PolicySpecifiedPasswordReuse params;
  params.url = url.spec();
  params.user_name = user_name;
  params.is_phishing_url = is_phishing_url;

  if (event_router_) {
    auto event_value = std::make_unique<base::ListValue>();
    event_value->Append(params.ToValue());

    auto extension_event = std::make_unique<Event>(
        events::
            SAFE_BROWSING_PRIVATE_ON_POLICY_SPECIFIED_PASSWORD_REUSE_DETECTED,
        api::safe_browsing_private::OnPolicySpecifiedPasswordReuseDetected::
            kEventName,
        std::move(event_value));
    event_router_->BroadcastEvent(std::move(extension_event));
  }

  if (client_) {
    base::Value event(base::Value::Type::DICTIONARY);
    event.SetStringKey(kKeyUrl, params.url);
    event.SetStringKey(kKeyUserName, params.user_name);
    event.SetBoolKey(kKeyIsPhishingUrl, params.is_phishing_url);
    event.SetStringKey(kKeyProfileUserName, GetProfileUserName());
    ReportRealtimeEvent(kKeyPasswordReuseEvent, std::move(event));
  }
}
