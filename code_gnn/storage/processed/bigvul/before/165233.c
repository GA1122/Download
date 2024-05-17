void ArcVoiceInteractionFrameworkService::ShowMetalayer() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  NotifyMetalayerStatusChanged(true);
}
