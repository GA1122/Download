QString WebContext::userAgent() const {
  if (IsInitialized()) {
    return QString::fromStdString(
        UserAgentSettings::Get(context_.get())->GetUserAgent());
  }

  return QString::fromStdString(construct_props_->user_agent);
}
