ArcVoiceInteractionFrameworkService::ArcVoiceInteractionFrameworkService(
    content::BrowserContext* context,
    ArcBridgeService* bridge_service)
    : context_(context),
      arc_bridge_service_(bridge_service),
      highlighter_client_(std::make_unique<HighlighterControllerClient>(this)),
      weak_ptr_factory_(this) {
  arc_bridge_service_->voice_interaction_framework()->SetHost(this);
  arc_bridge_service_->voice_interaction_framework()->AddObserver(this);
  ArcSessionManager::Get()->AddObserver(this);
  chromeos::CrasAudioHandler::Get()->AddAudioObserver(this);
}
