void EnterpriseEnrollmentScreen::WriteInstallAttributesData() {
  runnable_method_factory_.RevokeAll();

  switch (g_browser_process->browser_policy_connector()->LockDevice(user_)) {
    case policy::EnterpriseInstallAttributes::LOCK_SUCCESS: {
      policy::BrowserPolicyConnector* connector =
          g_browser_process->browser_policy_connector();
      connector->FetchDevicePolicy();
      return;
    }
    case policy::EnterpriseInstallAttributes::LOCK_NOT_READY: {
      LOG(WARNING) << "Install Attributes not ready yet will retry in "
                   << kLockRetryIntervalMs << "ms.";
      MessageLoop::current()->PostDelayedTask(
          FROM_HERE,
          runnable_method_factory_.NewRunnableMethod(
              &EnterpriseEnrollmentScreen::WriteInstallAttributesData),
          kLockRetryIntervalMs);
      return;
    }
    case policy::EnterpriseInstallAttributes::LOCK_BACKEND_ERROR: {
      actor_->ShowFatalEnrollmentError();
      return;
    }
    case policy::EnterpriseInstallAttributes::LOCK_WRONG_USER: {
      LOG(ERROR) << "Enrollment can not proceed because the InstallAttrs "
                 << "has been locked already!";
      actor_->ShowFatalEnrollmentError();
      return;
    }
  }

  NOTREACHED();
}
