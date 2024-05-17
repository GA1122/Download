ArcVoiceInteractionFrameworkService::~ArcVoiceInteractionFrameworkService() {
  chromeos::CrasAudioHandler::Get()->RemoveAudioObserver(this);
  ArcSessionManager::Get()->RemoveObserver(this);
  arc_bridge_service_->voice_interaction_framework()->RemoveObserver(this);
  arc_bridge_service_->voice_interaction_framework()->SetHost(nullptr);
}
