void OneClickSigninSyncStarter::CompleteInitForNewProfile(
    chrome::HostDesktopType desktop_type,
    Profile* new_profile,
    Profile::CreateStatus status) {
  DCHECK_NE(profile_, new_profile);

  switch (status) {
    case Profile::CREATE_STATUS_LOCAL_FAIL: {
      NOTREACHED() << "Error creating new profile";
      CancelSigninAndDelete();
      return;
    }
    case Profile::CREATE_STATUS_CREATED: {
      break;
    }
    case Profile::CREATE_STATUS_INITIALIZED: {
      SigninManager* old_signin_manager =
          SigninManagerFactory::GetForProfile(profile_);
      SigninManager* new_signin_manager =
          SigninManagerFactory::GetForProfile(new_profile);
      DCHECK(!old_signin_manager->GetUsernameForAuthInProgress().empty());
      DCHECK(old_signin_manager->GetAuthenticatedUsername().empty());
      DCHECK(new_signin_manager->GetAuthenticatedUsername().empty());
      DCHECK(policy_client_);

      new_signin_manager->CopyCredentialsFrom(*old_signin_manager);
      FinishProfileSyncServiceSetup();
      Initialize(new_profile, NULL);
      DCHECK_EQ(profile_, new_profile);

      old_signin_manager->SignOut();

      LoadPolicyWithCachedClient();

      ProfileManager::FindOrCreateNewWindowForProfile(
        new_profile,
        chrome::startup::IS_PROCESS_STARTUP,
        chrome::startup::IS_FIRST_RUN,
        desktop_type,
        false);
      break;
    }
    case Profile::CREATE_STATUS_REMOTE_FAIL:
    case Profile::CREATE_STATUS_CANCELED:
    case Profile::MAX_CREATE_STATUS: {
      NOTREACHED() << "Invalid profile creation status";
      CancelSigninAndDelete();
      return;
    }
  }
}
