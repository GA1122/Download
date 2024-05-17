void WebContext::setProduct(const QString& product) {
  if (IsInitialized()) {
    UserAgentSettings::Get(context_.get())->SetProduct(
        product.toStdString());
  } else {
    construct_props_->product = product.toStdString();
  }
}
