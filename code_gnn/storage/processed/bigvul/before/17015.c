void WebContext::setUserAgentOverrides(
    const QList<UserAgentOverride>& overrides) {
  std::vector<UserAgentSettings::UserAgentOverride> o;
  for (auto it = overrides.begin(); it != overrides.end(); ++it) {
    o.push_back(
        std::make_pair((*it).first.toStdString(),
                       (*it).second.toStdString()));
  }

  if (IsInitialized()) {
    UserAgentSettings::Get(context_.get())->SetUserAgentOverrides(o);
  } else {
    construct_props_->user_agent_overrides = o;
  }
}
