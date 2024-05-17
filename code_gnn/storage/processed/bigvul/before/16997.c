void WebContext::setAllowedExtraUrlSchemes(const QStringList& schemes) {
  std::set<std::string> set;
  for (int i = 0; i < schemes.size(); ++i) {
    set.insert(base::ToLowerASCII(schemes.at(i).toStdString()));
  }
  delegate_->SetAllowedExtraURLSchemes(set);
}
