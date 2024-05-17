void ArcVoiceInteractionFrameworkService::StartVoiceInteractionOobe() {
  if (chromeos::LoginDisplayHost::default_host())
    return;
  chromeos::LoginDisplayHostWebUI* display_host =
      new chromeos::LoginDisplayHostWebUI();
  display_host->StartVoiceInteractionOobe();
}
