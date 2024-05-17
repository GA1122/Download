void ChromotingInstance::HandleReleaseAllKeys(
    const base::DictionaryValue& data) {
  if (IsConnected())
    input_tracker_.ReleaseAll();
}
