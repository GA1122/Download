  bool Init(HWND owner) {
    const int kMaxAttemptsToOpenClipboard = 5;
    const base::TimeDelta kSleepTimeBetweenAttempts =
        base::TimeDelta::FromMilliseconds(5);

    if (opened_) {
      NOTREACHED();
      return true;
    }

    for (int attempt = 0; attempt < kMaxAttemptsToOpenClipboard; ++attempt) {
      if (attempt > 0) {
        base::PlatformThread::Sleep(kSleepTimeBetweenAttempts);
      }
      if (::OpenClipboard(owner)) {
        opened_ = true;
        return true;
      }
    }
    return false;
  }
