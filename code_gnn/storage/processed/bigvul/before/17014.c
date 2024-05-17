void WebContext::setUserAgent(const QString& user_agent) {
  if (IsInitialized()) {
    UserAgentSettings::Get(context_.get())->SetUserAgent(
        user_agent.toStdString());
  } else {
    construct_props_->user_agent = user_agent.toStdString();
  }
}
