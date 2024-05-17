void HistogramEnumerateLoadStatus(PluginErrorCode error_code) {
  HistogramEnumerate("NaCl.LoadStatus.Plugin", error_code, ERROR_MAX,
                     ERROR_UNKNOWN);
}
