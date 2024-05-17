void BrowserEventRouter::DispatchEventToExtension(
    Profile* profile,
    const std::string& extension_id,
    const char* event_name,
    scoped_ptr<ListValue> event_args,
    EventRouter::UserGestureState user_gesture) {
  if (!profile_->IsSameProfile(profile) ||
      !extensions::ExtensionSystem::Get(profile)->event_router())
    return;

  extensions::ExtensionSystem::Get(profile)->event_router()->
      DispatchEventToExtension(extension_id, event_name, event_args.Pass(),
                               profile, GURL(), user_gesture);
}
