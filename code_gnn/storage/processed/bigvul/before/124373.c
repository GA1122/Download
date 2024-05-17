void BrowserEventRouter::TabCreatedAt(WebContents* contents,
                                      int index,
                                      bool active) {
  Profile* profile = Profile::FromBrowserContext(contents->GetBrowserContext());
  scoped_ptr<ListValue> args(new ListValue());
  scoped_ptr<Event> event(new Event(events::kOnTabCreated, args.Pass()));
  event->restrict_to_profile = profile;
  event->user_gesture = EventRouter::USER_GESTURE_NOT_ENABLED;
  event->will_dispatch_callback =
      base::Bind(&WillDispatchTabCreatedEvent, contents, active);
  ExtensionSystem::Get(profile)->event_router()->BroadcastEvent(event.Pass());

  RegisterForTabNotifications(contents);
}
