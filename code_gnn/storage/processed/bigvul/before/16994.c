QString WebContext::product() const {
  if (IsInitialized()) {
    return QString::fromStdString(
        UserAgentSettings::Get(context_.get())->GetProduct());
  }

  return QString::fromStdString(construct_props_->product);
}
