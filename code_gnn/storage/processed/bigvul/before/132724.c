void ChromotingInstance::HandleOnIncomingIq(const base::DictionaryValue& data) {
  std::string iq;
  if (!data.GetString("iq", &iq)) {
    LOG(ERROR) << "Invalid incomingIq() data.";
    return;
  }

  if (signal_strategy_)
    signal_strategy_->OnIncomingMessage(iq);
}
