 bool ExtensionTtsPlatformImplChromeOs::StopSpeaking() {
  if (chromeos::CrosLibrary::Get()->EnsureLoaded()) {
    return chromeos::CrosLibrary::Get()->GetSpeechSynthesisLibrary()->
        StopSpeaking();
  }

  set_error(kCrosLibraryNotLoadedError);
   return false;
 }
