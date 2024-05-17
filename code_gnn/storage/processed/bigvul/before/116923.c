Browser* SessionRestore::RestoreSession(Profile* profile,
                                        Browser* browser,
                                        uint32 behavior,
                                        const std::vector<GURL>& urls_to_open) {
#if defined(OS_CHROMEOS)
  chromeos::BootTimesLoader::Get()->AddLoginTimeMarker(
      "SessionRestore-Start", false);
#endif
  DCHECK(profile);
  profile = profile->GetOriginalProfile();
  if (!SessionServiceFactory::GetForProfile(profile)) {
    NOTREACHED();
    return NULL;
  }
  profile->set_restored_last_session(true);
  SessionRestoreImpl* restorer = new SessionRestoreImpl(
      profile, browser, (behavior & SYNCHRONOUS) != 0,
      (behavior & CLOBBER_CURRENT_TAB) != 0,
      (behavior & ALWAYS_CREATE_TABBED_BROWSER) != 0,
      urls_to_open);
  return restorer->Restore();
}
