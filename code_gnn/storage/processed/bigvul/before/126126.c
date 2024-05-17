void BrowserEventRouter::TabCreatedAt(WebContents* contents,
                                      int index,
                                      bool active) {
  Profile* profile = Profile::FromBrowserContext(contents->GetBrowserContext());
  const EventListenerMap::ListenerList& listeners(
      ExtensionSystem::Get(profile)->event_router()->
      listeners().GetEventListenersByName(events::kOnTabCreated));
  for (EventListenerMap::ListenerList::const_iterator it = listeners.begin();
       it != listeners.end();
       ++it) {
    scoped_ptr<ListValue> args(new ListValue());
    DictionaryValue* tab_value = ExtensionTabUtil::CreateTabValue(
        contents,
        profile->GetExtensionService()->extensions()->GetByID(
            (*it)->extension_id));
    args->Append(tab_value);
    tab_value->SetBoolean(tab_keys::kSelectedKey, active);
    DispatchEventToExtension(profile, (*it)->extension_id,
                             events::kOnTabCreated, args.Pass(),
                             EventRouter::USER_GESTURE_NOT_ENABLED);
  }

  RegisterForTabNotifications(contents);
}
