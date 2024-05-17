void BookmarkEventRouter::DispatchEvent(
    Profile* profile,
    const char* event_name,
    scoped_ptr<ListValue> event_args) {
  if (extensions::ExtensionSystem::Get(profile)->event_router()) {
    extensions::ExtensionSystem::Get(profile)->event_router()->BroadcastEvent(
        make_scoped_ptr(new extensions::Event(event_name, event_args.Pass())));
  }
}
