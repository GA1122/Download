void PeopleHandler::HandleManageOtherPeople(const base::ListValue*  ) {
#if !defined(OS_CHROMEOS)
  UserManager::Show(base::FilePath(),
                    profiles::USER_MANAGER_SELECT_PROFILE_NO_ACTION);
#endif   
}
