void ArcVoiceInteractionFrameworkService::SetVoiceInteractionSetupCompleted() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  SetVoiceInteractionSetupCompletedInternal(true);
  SetVoiceInteractionEnabled(true, base::BindOnce(&DoNothing<bool>));
  SetVoiceInteractionContextEnabled(true);
}
