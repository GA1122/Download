void ChromotingInstance::OnVideoFirstFrameReceived() {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  PostLegacyJsonMessage("onFirstFrameReceived", data.Pass());
}
