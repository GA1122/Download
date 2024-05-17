void ArcVoiceInteractionFrameworkService::NotifyMetalayerStatusChanged(
    bool visible) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  mojom::VoiceInteractionFrameworkInstance* framework_instance =
      ARC_GET_INSTANCE_FOR_METHOD(
          arc_bridge_service_->voice_interaction_framework(),
          SetMetalayerVisibility);
  if (!framework_instance)
    return;
  framework_instance->SetMetalayerVisibility(visible);
}
