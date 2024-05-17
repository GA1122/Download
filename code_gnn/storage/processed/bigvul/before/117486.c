void TestingAutomationProvider::SetMute(DictionaryValue* args,
                                        IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  bool mute;
  if (!args->GetBoolean("mute", &mute)) {
    reply.SendError("Invalid or missing args.");
    return;
  }
  chromeos::AudioHandler* audio_handler = chromeos::AudioHandler::GetInstance();
  if (!audio_handler) {
    reply.SendError("AudioHandler not initialized.");
    return;
  }
  audio_handler->SetMuted(mute);
  reply.SendSuccess(NULL);
}
