void ExtensionDevToolsClientHost::SendDetachedEvent() {
  if (!EventRouter::Get(profile_))
    return;

  scoped_ptr<base::ListValue> args(OnDetach::Create(debuggee_,
                                                    detach_reason_));
  scoped_ptr<Event> event(new Event(OnDetach::kEventName, args.Pass()));
  event->restrict_to_browser_context = profile_;
  EventRouter::Get(profile_)
      ->DispatchEventToExtension(extension_id_, event.Pass());
}
