bool IsInKioskMode() {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode))
    return true;

#if defined(OS_CHROMEOS)
  const chromeos::UserManager* user_manager = chromeos::UserManager::Get();
  return user_manager && user_manager->IsLoggedInAsKioskApp();
#else
  return false;
#endif
}
