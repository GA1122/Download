void MediaStreamDevicesController::RegisterUserPrefs(
    user_prefs::PrefRegistrySyncable* prefs) {
  prefs->RegisterBooleanPref(prefs::kVideoCaptureAllowed,
                             true,
                             user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  prefs->RegisterBooleanPref(prefs::kAudioCaptureAllowed,
                             true,
                             user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  prefs->RegisterListPref(prefs::kVideoCaptureAllowedUrls,
                          user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  prefs->RegisterListPref(prefs::kAudioCaptureAllowedUrls,
                          user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
}
