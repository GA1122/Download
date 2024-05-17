void ResetScreenHandler::RegisterMessages() {
  AddCallback("cancelOnReset", &ResetScreenHandler::HandleOnCancel);
  AddCallback("restartOnReset", &ResetScreenHandler::HandleOnRestart);
  AddCallback("powerwashOnReset", &ResetScreenHandler::HandleOnPowerwash);
  AddCallback("resetOnLearnMore", &ResetScreenHandler::HandleOnLearnMore);
}
