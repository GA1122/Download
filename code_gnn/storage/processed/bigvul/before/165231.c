    SetVoiceInteractionSetupCompletedInternal(bool completed) {
  PrefService* prefs = Profile::FromBrowserContext(context_)->GetPrefs();
  prefs->SetBoolean(prefs::kArcVoiceInteractionValuePropAccepted, completed);
}
