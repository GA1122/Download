void ArcVoiceInteractionFrameworkService::OnArcPlayStoreEnabledChanged(
    bool enabled) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  if (enabled)
    return;

  SetVoiceInteractionSetupCompletedInternal(false);
  SetVoiceInteractionEnabled(false, base::BindOnce(&DoNothing<bool>));
  SetVoiceInteractionContextEnabled(false);
}
