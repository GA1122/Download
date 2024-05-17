void TestingAutomationProvider::SetVolume(DictionaryValue* args,
                                          IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  double volume_percent;
  if (!args->GetDouble("volume", &volume_percent)) {
    reply.SendError("Invalid or missing args.");
    return;
  }
  chromeos::AudioHandler* audio_handler = chromeos::AudioHandler::GetInstance();
  if (!audio_handler) {
    reply.SendError("AudioHandler not initialized.");
    return;
  }
  audio_handler->SetVolumePercent(volume_percent);
  reply.SendSuccess(NULL);
}
