void RecordSettingsEnabledState(
    data_reduction_proxy::DataReductionSettingsEnabledAction action) {
  UMA_HISTOGRAM_ENUMERATION(
      "DataReductionProxy.EnabledState", action,
      data_reduction_proxy::DATA_REDUCTION_SETTINGS_ACTION_BOUNDARY);
}
