void HistogramEnumerateSelLdrLoadStatus(NaClErrorCode error_code) {
  HistogramEnumerate("NaCl.LoadStatus.SelLdr", error_code, NACL_ERROR_CODE_MAX,
                     LOAD_STATUS_UNKNOWN);
}
