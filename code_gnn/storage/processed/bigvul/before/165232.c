void ArcVoiceInteractionFrameworkService::SetVoiceInteractionState(
    arc::mojom::VoiceInteractionState voice_interaction_state) {
  ash::mojom::VoiceInteractionState state =
      mojo::ConvertTo<ash::mojom::VoiceInteractionState>(
          voice_interaction_state);
  DCHECK_NE(state_, state);
  if (state_ == ash::mojom::VoiceInteractionState::NOT_READY) {
    PrefService* prefs = Profile::FromBrowserContext(context_)->GetPrefs();
    bool value_prop_accepted =
        prefs->GetBoolean(prefs::kArcVoiceInteractionValuePropAccepted);

    bool enable_voice_interaction =
        value_prop_accepted &&
        prefs->GetBoolean(prefs::kVoiceInteractionEnabled);
    SetVoiceInteractionEnabled(enable_voice_interaction,
                               base::BindOnce(&DoNothing<bool>));

    SetVoiceInteractionContextEnabled(
        enable_voice_interaction &&
        prefs->GetBoolean(prefs::kVoiceInteractionContextEnabled));
  }

  if (state_ == ash::mojom::VoiceInteractionState::RUNNING)
    highlighter_client_->Exit();

  state_ = state;
  arc::VoiceInteractionControllerClient::Get()->NotifyStatusChanged(state);
}
