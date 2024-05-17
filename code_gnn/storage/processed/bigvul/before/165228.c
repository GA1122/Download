void ArcVoiceInteractionFrameworkService::SetVoiceInteractionContextEnabled(
    bool enable) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  PrefService* prefs = Profile::FromBrowserContext(context_)->GetPrefs();
  prefs->SetBoolean(prefs::kVoiceInteractionContextEnabled, enable);

  mojom::VoiceInteractionFrameworkInstance* framework_instance =
      ARC_GET_INSTANCE_FOR_METHOD(
          arc_bridge_service_->voice_interaction_framework(),
          SetVoiceInteractionContextEnabled);
  if (!framework_instance)
    return;
  framework_instance->SetVoiceInteractionContextEnabled(enable);
}
