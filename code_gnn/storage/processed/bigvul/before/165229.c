void ArcVoiceInteractionFrameworkService::SetVoiceInteractionEnabled(
    bool enable,
    VoiceInteractionSettingCompleteCallback callback) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  PrefService* prefs = Profile::FromBrowserContext(context_)->GetPrefs();

  prefs->SetBoolean(prefs::kVoiceInteractionEnabled, enable);
  if (!enable)
    prefs->SetBoolean(prefs::kVoiceInteractionContextEnabled, false);

  mojom::VoiceInteractionFrameworkInstance* framework_instance =
      ARC_GET_INSTANCE_FOR_METHOD(
          arc_bridge_service_->voice_interaction_framework(),
          SetVoiceInteractionEnabled);
  if (!framework_instance) {
    std::move(callback).Run(false);
    return;
  }
  framework_instance->SetVoiceInteractionEnabled(
      enable, base::BindOnce(std::move(callback), true));
}
