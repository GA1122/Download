void OSExchangeData::SetPickledData(const CustomFormat& format,
                                    const Pickle& data) {
  provider_->SetPickledData(format, data);
}
