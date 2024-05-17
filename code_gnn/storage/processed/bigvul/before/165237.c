void ArcVoiceInteractionFrameworkService::StartVoiceInteractionSetupWizard() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  DCHECK(ARC_GET_INSTANCE_FOR_METHOD(
      arc_bridge_service_->voice_interaction_framework(),
      StartVoiceInteractionSession));
  SetVoiceInteractionEnabled(
      true, base::BindOnce(
                [](base::OnceClosure next, bool setting_applied) {
                  if (!setting_applied)
                    DVLOG(1) << "Not synchronizing settings: version mismatch";
                  std::move(next).Run();
                },
                base::BindOnce(&ArcVoiceInteractionFrameworkService::
                                   StartVoiceInteractionSetupWizardActivity,
                               weak_ptr_factory_.GetWeakPtr())));
}
