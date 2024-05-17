bool ArcVoiceInteractionFrameworkService::InitiateUserInteraction(
    bool is_toggle) {
  VLOG(1) << "Start voice interaction.";
  PrefService* prefs = Profile::FromBrowserContext(context_)->GetPrefs();
  if (!prefs->GetBoolean(prefs::kArcVoiceInteractionValuePropAccepted)) {
    VLOG(1) << "Voice interaction feature not accepted.";
    should_start_runtime_flow_ = true;
    StartVoiceInteractionOobe();
    return false;
  }

  if (!prefs->GetBoolean(prefs::kVoiceInteractionEnabled))
    return false;

  if (state_ == ash::mojom::VoiceInteractionState::NOT_READY) {
    arc::VoiceInteractionControllerClient::Get()->NotifyStatusChanged(
        ash::mojom::VoiceInteractionState::NOT_READY);
  }

  ArcBootPhaseMonitorBridge::RecordFirstAppLaunchDelayUMA(context_);
  if (!arc_bridge_service_->voice_interaction_framework()->IsConnected()) {
    VLOG(1) << "Instance not ready.";
    SetArcCpuRestriction(false);
    is_request_pending_ = true;
    is_pending_request_toggle_ = is_toggle;
    return false;
  }

  user_interaction_start_time_ = base::TimeTicks::Now();
  context_request_remaining_count_ = kContextRequestMaxRemainingCount;
  return true;
}
