void WebContext::setAcceptLangs(const QString& langs) {
  if (IsInitialized()) {
    UserAgentSettings::Get(context_.get())->SetAcceptLangs(
        langs.toStdString());
  } else {
    construct_props_->accept_langs = langs.toStdString();
  }
}
