void ArcVoiceInteractionFrameworkService::StartSessionFromUserInteraction(
    const gfx::Rect& rect) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (!InitiateUserInteraction(false  ))
    return;

  if (rect.IsEmpty()) {
    mojom::VoiceInteractionFrameworkInstance* framework_instance =
        ARC_GET_INSTANCE_FOR_METHOD(
            arc_bridge_service_->voice_interaction_framework(),
            StartVoiceInteractionSession);
    DCHECK(framework_instance);
    framework_instance->StartVoiceInteractionSession(IsHomescreenActive());
  } else {
    mojom::VoiceInteractionFrameworkInstance* framework_instance =
        ARC_GET_INSTANCE_FOR_METHOD(
            arc_bridge_service_->voice_interaction_framework(),
            StartVoiceInteractionSessionForRegion);
    DCHECK(framework_instance);
    framework_instance->StartVoiceInteractionSessionForRegion(rect);
  }
  VLOG(1) << "Sent voice interaction request.";
}
