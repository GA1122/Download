void AudioMixerAlsa::RegisterPrefs(PrefService* local_state) {
  if (!local_state->FindPreference(prefs::kAudioVolume))
    local_state->RegisterDoublePref(prefs::kAudioVolume,
                                    kDefaultVolumeDb,
                                    PrefService::UNSYNCABLE_PREF);
  if (!local_state->FindPreference(prefs::kAudioMute))
    local_state->RegisterIntegerPref(prefs::kAudioMute,
                                     kPrefMuteOff,
                                     PrefService::UNSYNCABLE_PREF);
}
