NavigationObserver::NavigationObserver(Profile* profile)
    : profile_(profile),
      extension_registry_observer_(this),
      weak_factory_(this) {
  RegisterForNotifications();
  extension_registry_observer_.Add(ExtensionRegistry::Get(profile));
}
