  std::wstring GetUsageStatsKeyPath(bool medium) {
    EXPECT_TRUE(!medium || system_level_);

    std::wstring result(L"Software\\");
    if (kUseGoogleUpdateIntegration) {
      result.append(L"Google\\Update\\ClientState");
      if (medium)
        result.append(L"Medium");
      result.push_back(L'\\');
      result.append(mode_->app_guid);
    } else {
      result.append(kProductPathName);
    }
    return result;
  }
