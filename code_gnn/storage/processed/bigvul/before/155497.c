void DataReductionProxySettings::AddDataReductionProxySettingsObserver(
    DataReductionProxySettingsObserver* observer) {
  DCHECK(thread_checker_.CalledOnValidThread());
  observers_.AddObserver(observer);
}
