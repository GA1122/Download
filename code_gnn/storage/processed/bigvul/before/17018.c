WebContext::userAgentOverrides() const {
  QList<UserAgentOverride> rv;

  std::vector<UserAgentSettings::UserAgentOverride> overrides;
  if (IsInitialized()) {
    overrides =
        UserAgentSettings::Get(context_.get())->GetUserAgentOverrides();
  } else {
    overrides = construct_props_->user_agent_overrides;
  }

  for (const auto& entry : overrides) {
    rv.append(
        qMakePair(QString::fromStdString(entry.first),
                  QString::fromStdString(entry.second)));
  }

  return rv;
}
