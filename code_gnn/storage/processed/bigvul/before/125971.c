NewTabUILoadObserver::NewTabUILoadObserver(AutomationProvider* automation,
                                           Profile* profile)
    : automation_(automation->AsWeakPtr()) {
  registrar_.Add(this, chrome::NOTIFICATION_INITIAL_NEW_TAB_UI_LOAD,
                 content::Source<Profile>(profile));
}
