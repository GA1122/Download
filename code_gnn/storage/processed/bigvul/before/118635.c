  void PrepareProfile(const std::string& username) {
    ChromeBrowserMainExtraPartsProfiles::
        EnsureBrowserContextKeyedServiceFactoriesBuilt();

    DeviceSettingsTestHelper device_settings_test_helper;
    DeviceSettingsService::Get()->SetSessionManager(
        &device_settings_test_helper, new MockOwnerKeyUtil());

    EXPECT_CALL(*mock_async_method_caller_, AsyncMount(_, _, _, _))
        .WillRepeatedly(Return());
    EXPECT_CALL(*mock_async_method_caller_, AsyncGetSanitizedUsername(_, _))
        .WillRepeatedly(Return());

    scoped_refptr<Authenticator> authenticator =
        LoginUtils::Get()->CreateAuthenticator(this);
    UserContext user_context(username);
    user_context.SetKey(Key("password"));
    user_context.SetUserIDHash(username);
    authenticator->CompleteLogin(ProfileHelper::GetSigninProfile(),
                                 user_context);

    const bool kHasAuthCookies = false;
    const bool kHasActiveSession = false;
    user_context.SetAuthFlow(UserContext::AUTH_FLOW_GAIA_WITHOUT_SAML);
    LoginUtils::Get()->PrepareProfile(user_context,
                                      kHasAuthCookies,
                                      kHasActiveSession,
                                      this);
    device_settings_test_helper.Flush();
    RunUntilIdle();

    DeviceSettingsService::Get()->UnsetSessionManager();
  }
