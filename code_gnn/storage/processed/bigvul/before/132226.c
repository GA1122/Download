void MaybeHandleDebugURL(const GURL& url) {
  if (!url.SchemeIs(kChromeUIScheme))
    return;
  if (url == GURL(kChromeUIBadCastCrashURL)) {
    BadCastCrashIntentionally();
  } else if (url == GURL(kChromeUICrashURL)) {
    CrashIntentionally();
  } else if (url == GURL(kChromeUIDumpURL)) {
    base::debug::DumpWithoutCrashing();
  } else if (url == GURL(kChromeUIKillURL)) {
    base::Process::Current().Terminate(1, false);
  } else if (url == GURL(kChromeUIHangURL)) {
    for (;;) {
      base::PlatformThread::Sleep(base::TimeDelta::FromSeconds(1));
    }
  } else if (url == GURL(kChromeUIShorthangURL)) {
    base::PlatformThread::Sleep(base::TimeDelta::FromSeconds(20));
  }

#if defined(ADDRESS_SANITIZER) || defined(SYZYASAN)
  MaybeTriggerAsanError(url);
#endif   
}
