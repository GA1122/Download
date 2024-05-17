void DataReductionProxySettings::OnProxyEnabledPrefChange() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!register_synthetic_field_trial_.is_null()) {
    RegisterDataReductionProxyFieldTrial();
  }
  MaybeActivateDataReductionProxy(false);

  bool enabled = IsDataReductionProxyEnabled();
  for (auto& observer : observers_)
    observer.OnDataSaverEnabledChanged(enabled);
}
