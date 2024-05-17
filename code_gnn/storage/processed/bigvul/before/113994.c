  void SetProfile(Profile* profile) {
    pref_registrar_.reset(new PrefChangeRegistrar);
    pref_registrar_->Init(profile->GetPrefs());
    pref_registrar_->Add(prefs::kUse24HourClock, this);
    pref_registrar_->Add(prefs::kLanguageXkbRemapSearchKeyTo, this);
    UpdateClockType(profile->GetPrefs());
    search_key_mapped_to_ =
        profile->GetPrefs()->GetInteger(prefs::kLanguageXkbRemapSearchKeyTo);

    if (gdata::util::IsGDataAvailable(profile)) {
      GDataSystemService* system_service =
          GDataSystemServiceFactory::FindForProfile(profile);
      if (!system_service || !system_service->file_system())
        return;

      system_service->file_system()->GetOperationRegistry()->
          AddObserver(this);
    }
  }
