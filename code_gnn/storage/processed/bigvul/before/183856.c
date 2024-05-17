  bool ExtensionTtsPlatformImplChromeOs::Speak(
//     int utterance_id,
      const std::string& utterance,
    const std::string& locale,
    const std::string& gender,
    double rate,
    double pitch,
    double volume) {
//     const std::string& lang,
//     const UtteranceContinuousParameters& params) {
    chromeos::CrosLibrary* cros_library = chromeos::CrosLibrary::Get();
    if (!cros_library->EnsureLoaded()) {
      set_error(kCrosLibraryNotLoadedError);
      return false;
    }
  
//   utterance_id_ = utterance_id;
//   utterance_length_ = utterance.size();
// 
    std::string options;
  
  if (!locale.empty()) {
//   if (!lang.empty()) {
      AppendSpeakOption(
          chromeos::SpeechSynthesisLibrary::kSpeechPropertyLocale,
        locale,
//         lang,
          &options);
    }
  
  if (!gender.empty()) {
    AppendSpeakOption(
        chromeos::SpeechSynthesisLibrary::kSpeechPropertyGender,
        gender,
        &options);
  }
  if (rate >= 0.0) {
//   if (params.rate >= 0.0) {
      AppendSpeakOption(
          chromeos::SpeechSynthesisLibrary::kSpeechPropertyRate,
        DoubleToString(rate * 5),
//         DoubleToString(1.5 + params.rate * 2.5),
          &options);
    }
  
  if (pitch >= 0.0) {
//   if (params.pitch >= 0.0) {
      AppendSpeakOption(
          chromeos::SpeechSynthesisLibrary::kSpeechPropertyPitch,
        DoubleToString(pitch * 2),
//         DoubleToString(params.pitch),
          &options);
    }
  
  if (volume >= 0.0) {
//   if (params.volume >= 0.0) {
      AppendSpeakOption(
          chromeos::SpeechSynthesisLibrary::kSpeechPropertyVolume,
        DoubleToString(volume * 5),
//         DoubleToString(params.volume * 5),
          &options);
    }
  
   if (!options.empty()) {
     cros_library->GetSpeechSynthesisLibrary()->SetSpeakProperties(
          options.c_str());
    }
  
  return cros_library->GetSpeechSynthesisLibrary()->Speak(utterance.c_str());
//   bool result =
//       cros_library->GetSpeechSynthesisLibrary()->Speak(utterance.c_str());
// 
//   if (result) {
//     ExtensionTtsController* controller = ExtensionTtsController::GetInstance();
//     controller->OnTtsEvent(utterance_id_, TTS_EVENT_START, 0, std::string());
//     PollUntilSpeechFinishes(utterance_id_);
//   }
// 
//   return result;
  }