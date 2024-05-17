void ReportFoundUwS() {
  base::win::RegKey reporter_key(HKEY_CURRENT_USER,
                                 kSoftwareRemovalToolRegistryKey,
                                 KEY_QUERY_VALUE | KEY_SET_VALUE);
  std::vector<base::string16> found_uws_strings;
  if (reporter_key.Valid() &&
      reporter_key.ReadValues(kFoundUwsValueName, &found_uws_strings) ==
          ERROR_SUCCESS) {
    rappor::RapporService* rappor_service = g_browser_process->rappor_service();

    bool parse_error = false;
    for (const base::string16& uws_string : found_uws_strings) {
      uint32_t uws_id = 0;
      if (base::StringToUint(uws_string, &uws_id)) {
        UMA_HISTOGRAM_SPARSE_SLOWLY(kFoundUwsMetricName, uws_id);
        if (rappor_service) {
          rappor_service->RecordSample(kFoundUwsMetricName,
                                       rappor::COARSE_RAPPOR_TYPE,
                                       base::UTF16ToUTF8(uws_string));
        }
      } else {
        parse_error = true;
      }
    }

    reporter_key.DeleteValue(kFoundUwsValueName);

    UMA_HISTOGRAM_BOOLEAN(kFoundUwsReadErrorMetricName, parse_error);
  }
}
