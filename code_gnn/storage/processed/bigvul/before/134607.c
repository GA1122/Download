OSExchangeData::Provider* OSExchangeDataProviderAura::Clone() const {
  OSExchangeDataProviderAura* ret = new OSExchangeDataProviderAura();
  ret->formats_ = formats_;
  ret->string_ = string_;
  ret->url_ = url_;
  ret->title_ = title_;
  ret->filenames_ = filenames_;
  ret->pickle_data_ = pickle_data_;
  ret->html_ = html_;
  ret->base_url_ = base_url_;

  return ret;
}
