void ArcVoiceInteractionFrameworkService::ShowVoiceInteractionSettings() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  arc::mojom::VoiceInteractionFrameworkInstance* framework_instance =
      ARC_GET_INSTANCE_FOR_METHOD(
          arc_bridge_service_->voice_interaction_framework(),
          ShowVoiceInteractionSettings);
  if (!framework_instance)
    return;
  framework_instance->ShowVoiceInteractionSettings();
}
