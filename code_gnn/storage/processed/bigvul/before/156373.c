void ExtensionDevToolsClientHost::SendDetachedEvent() {
  if (!EventRouter::Get(profile_))
    return;

  std::unique_ptr<base::ListValue> args(
      OnDetach::Create(debuggee_, detach_reason_));
  auto event =
      std::make_unique<Event>(events::DEBUGGER_ON_DETACH, OnDetach::kEventName,
                              std::move(args), profile_);
  EventRouter::Get(profile_)->DispatchEventToExtension(extension_id(),
                                                       std::move(event));
}
