 bool IsAppsShortcutEnabled(Profile* profile) {
  if (profile->IsLegacySupervised())
    return false;

#if defined(USE_ASH)
  return false;
#else
  return search::IsInstantExtendedAPIEnabled() && !profile->IsOffTheRecord();
#endif
}
