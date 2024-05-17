void SafeBrowsingPrivateEventRouter::OnPolicySpecifiedPasswordChanged(
    const std::string& user_name) {
  if (event_router_) {
    auto event_value = std::make_unique<base::ListValue>();
    event_value->Append(std::make_unique<base::Value>(user_name));
    auto extension_event = std::make_unique<Event>(
        events::SAFE_BROWSING_PRIVATE_ON_POLICY_SPECIFIED_PASSWORD_CHANGED,
        api::safe_browsing_private::OnPolicySpecifiedPasswordChanged::
            kEventName,
        std::move(event_value));
    event_router_->BroadcastEvent(std::move(extension_event));
  }

  if (client_) {
    base::Value event(base::Value::Type::DICTIONARY);
    event.SetStringKey(kKeyUserName, user_name);
    event.SetStringKey(kKeyProfileUserName, GetProfileUserName());
    ReportRealtimeEvent(kKeyPasswordChangedEvent, std::move(event));
  }
}
