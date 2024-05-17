void TestingAutomationProvider::GetVolumeInfo(DictionaryValue* args,
                                              IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  chromeos::AudioHandler* audio_handler = chromeos::AudioHandler::GetInstance();
  if (!audio_handler) {
    reply.SendError("AudioHandler not initialized.");
    return;
  }
  return_value->SetDouble("volume", audio_handler->GetVolumePercent());
  return_value->SetBoolean("is_mute", audio_handler->IsMuted());
  reply.SendSuccess(return_value.get());
}
