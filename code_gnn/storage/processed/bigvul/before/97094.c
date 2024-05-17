  wstring TestTabTitle(const wstring& value) {
    const int kWaitForErrorPageMsec = 200;

    for (int i = 0; i < 10; ++i) {
      if (value.compare(GetTabTitle()) == 0)
        return value;
      PlatformThread::Sleep(kWaitForErrorPageMsec);
    }
    return GetTabTitle();
  }
