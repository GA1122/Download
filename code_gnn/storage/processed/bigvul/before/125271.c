static void MaybeHandleDebugURL(const GURL& url) {
  if (!url.SchemeIs(chrome::kChromeUIScheme))
    return;
  if (url == GURL(chrome::kChromeUICrashURL)) {
    CrashIntentionally();
  } else if (url == GURL(chrome::kChromeUIKillURL)) {
    base::KillProcess(base::GetCurrentProcessHandle(), 1, false);
  } else if (url == GURL(chrome::kChromeUIHangURL)) {
    for (;;) {
      base::PlatformThread::Sleep(base::TimeDelta::FromSeconds(1));
    }
  } else if (url == GURL(kChromeUIShorthangURL)) {
    base::PlatformThread::Sleep(base::TimeDelta::FromSeconds(20));
  }
}
