bool ExtensionTtsIsSpeakingFunction::RunImpl() {
  result_.reset(Value::CreateBooleanValue(
      ExtensionTtsController::GetInstance()->IsSpeaking()));
   return true;
 }
