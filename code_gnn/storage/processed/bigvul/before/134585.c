bool OSExchangeData::GetPickledData(const CustomFormat& format,
                                    Pickle* data) const {
  return provider_->GetPickledData(format, data);
}
