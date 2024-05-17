void LoginDisplayHostWebUI::StartVoiceInteractionOobe() {
  is_voice_interaction_oobe_ = true;
  finalize_animation_type_ = ANIMATION_NONE;
  StartWizard(OobeScreen::SCREEN_VOICE_INTERACTION_VALUE_PROP);
  login_view_->set_should_emit_login_prompt_visible(false);
}
