void DataReductionProxySettings::RemoveDataReductionProxySettingsObserver(
    DataReductionProxySettingsObserver* observer) {
  DCHECK(thread_checker_.CalledOnValidThread());
  observers_.RemoveObserver(observer);
}
