void ShowUserManagerOnStartup(const base::CommandLine& command_line) {
#if !defined(OS_CHROMEOS)
  UserManager::Show(base::FilePath(),
                    profiles::USER_MANAGER_SELECT_PROFILE_NO_ACTION);
#endif   
}
