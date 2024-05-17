    StartVoiceInteractionSetupWizardActivity() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  arc::mojom::VoiceInteractionFrameworkInstance* framework_instance =
      ARC_GET_INSTANCE_FOR_METHOD(
          arc_bridge_service_->voice_interaction_framework(),
          StartVoiceInteractionSetupWizard);

  if (!framework_instance)
    return;

  if (should_start_runtime_flow_) {
    should_start_runtime_flow_ = false;
    VLOG(1) << "Starting runtime setup flow.";
    framework_instance->StartVoiceInteractionSession(IsHomescreenActive());
    return;
  }
   framework_instance->StartVoiceInteractionSetupWizard();
 }
