void CopyLockResult(base::RunLoop* loop,
                    policy::EnterpriseInstallAttributes::LockResult* out,
                    policy::EnterpriseInstallAttributes::LockResult result) {
  *out = result;
  loop->Quit();
}
