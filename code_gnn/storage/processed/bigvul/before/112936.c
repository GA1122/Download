bool IsGDataAvailable(Profile* profile) {
  if (!chromeos::UserManager::Get()->IsUserLoggedIn() ||
      chromeos::UserManager::Get()->IsLoggedInAsGuest() ||
      chromeos::UserManager::Get()->IsLoggedInAsDemoUser())
    return false;

  if (profile->IsOffTheRecord())
    return false;

  if (profile->GetPrefs()->GetBoolean(prefs::kDisableGData))
    return false;

  return true;
}
