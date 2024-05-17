QString WebContext::acceptLangs() const {
  if (IsInitialized()) {
    return QString::fromStdString(
        UserAgentSettings::Get(context_.get())->GetAcceptLangs());
  }

  return QString::fromStdString(construct_props_->accept_langs);
}
