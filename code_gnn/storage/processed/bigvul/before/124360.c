void BrowserEventRouter::DispatchEventToExtension(
    Profile* profile,
    const std::string& extension_id,
    const char* event_name,
    scoped_ptr<ListValue> event_args,
    EventRouter::UserGestureState user_gesture) {
  if (!profile_->IsSameProfile(profile) ||
      !extensions::ExtensionSystem::Get(profile)->event_router())
    return;

  scoped_ptr<Event> event(new Event(event_name, event_args.Pass()));
  event->restrict_to_profile = profile;
  event->user_gesture = user_gesture;
  ExtensionSystem::Get(profile)->event_router()->
      DispatchEventToExtension(extension_id, event.Pass());
}
