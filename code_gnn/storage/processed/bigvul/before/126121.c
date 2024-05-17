void BrowserEventRouter::DispatchTabUpdatedEvent(
    WebContents* contents, DictionaryValue* changed_properties) {
  DCHECK(changed_properties);
  DCHECK(contents);

  scoped_ptr<ListValue> args_base(new ListValue());

  args_base->AppendInteger(ExtensionTabUtil::GetTabId(contents));

  args_base->Append(changed_properties);

  Profile* profile = Profile::FromBrowserContext(contents->GetBrowserContext());

  const EventListenerMap::ListenerList& listeners(
      ExtensionSystem::Get(profile)->event_router()->
      listeners().GetEventListenersByName(events::kOnTabUpdated));
  for (EventListenerMap::ListenerList::const_iterator it = listeners.begin();
       it != listeners.end();
       ++it) {
    scoped_ptr<ListValue> args(args_base->DeepCopy());
    DictionaryValue* tab_value = ExtensionTabUtil::CreateTabValue(
        contents,
        profile->GetExtensionService()->extensions()->GetByID(
            (*it)->extension_id));
    args->Append(tab_value);
    DispatchEventToExtension(profile, (*it)->extension_id,
                             events::kOnTabUpdated, args.Pass(),
                             EventRouter::USER_GESTURE_UNKNOWN);
  }
}
