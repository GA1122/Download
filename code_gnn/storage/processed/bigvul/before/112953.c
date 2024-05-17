    GetAlternativeWebContentsToNotifyForDownload() {
#if defined(OS_ANDROID)
  return NULL;
#else
  Browser* last_active = browser::FindLastActiveWithProfile(profile_);
  return last_active ? chrome::GetActiveWebContents(last_active) : NULL;
#endif
}
