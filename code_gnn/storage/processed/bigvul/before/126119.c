void BrowserEventRouter::DispatchEventsAcrossIncognito(
    Profile* profile,
    const char* event_name,
    scoped_ptr<ListValue> event_args,
    scoped_ptr<ListValue> cross_incognito_args) {
  if (!profile_->IsSameProfile(profile) ||
      !extensions::ExtensionSystem::Get(profile)->event_router())
    return;

  extensions::ExtensionSystem::Get(profile)->event_router()->
      DispatchEventsToRenderersAcrossIncognito(event_name, event_args.Pass(),
          profile, cross_incognito_args.Pass(), GURL());
}
