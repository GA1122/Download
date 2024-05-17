  void EnrollDevice(const std::string& username) {
    base::RunLoop loop;
    policy::EnterpriseInstallAttributes::LockResult result;
    connector_->GetInstallAttributes()->LockDevice(
        username, policy::DEVICE_MODE_ENTERPRISE, kDeviceId,
        base::Bind(&CopyLockResult, &loop, &result));
    loop.Run();
    EXPECT_EQ(policy::EnterpriseInstallAttributes::LOCK_SUCCESS, result);
    RunUntilIdle();
  }
