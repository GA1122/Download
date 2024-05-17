void ArcVoiceInteractionFrameworkService::OnConnectionClosed() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  highlighter_client_->Detach();
}
