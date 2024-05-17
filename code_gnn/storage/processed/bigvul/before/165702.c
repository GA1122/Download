  void SetMetricsReportingPolicy(DWORD value) {
#if defined(GOOGLE_CHROME_BUILD)
    static constexpr wchar_t kPolicyKey[] =
        L"Software\\Policies\\Google\\Chrome";
#else
    static constexpr wchar_t kPolicyKey[] = L"Software\\Policies\\Chromium";
#endif

    ASSERT_EQ(ERROR_SUCCESS,
              base::win::RegKey(root_key_, kPolicyKey, KEY_SET_VALUE)
                  .WriteValue(L"MetricsReportingEnabled", value));
  }
