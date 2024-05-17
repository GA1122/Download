 bool ExtensionTtsStopSpeakingFunction::RunImpl() {
   ExtensionTtsController::GetInstance()->Stop();
   return true;
}
