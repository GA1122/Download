  void TestGenerationFallback(bool custom_passphrase_enabled) {
    MockSyncService mock_sync_service;
    EXPECT_CALL(mock_sync_service, IsFirstSetupComplete())
        .WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mock_sync_service, IsSyncActive())
        .WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mock_sync_service, GetActiveDataTypes())
        .Times(::testing::AnyNumber())
        .WillRepeatedly(
            ::testing::Return(syncer::ModelTypeSet(syncer::PASSWORDS)));
    EXPECT_CALL(mock_sync_service, IsUsingSecondaryPassphrase())
        .WillRepeatedly(::testing::Return(custom_passphrase_enabled));
    std::unique_ptr<TestPasswordManagerClient> client(
        new TestPasswordManagerClient);
    std::unique_ptr<MockAutofillClient> autofill_client(
        new MockAutofillClient(&mock_sync_service));
    InitializePasswordAutofillManager(client.get(), autofill_client.get());

    gfx::RectF element_bounds;
    autofill::PasswordFormFillData data;
    data.username_field.value = test_username_;
    data.password_field.value = test_password_;
    data.origin = GURL("https://foo.test");

    int dummy_key = 0;
    password_autofill_manager_->OnAddPasswordFormMapping(dummy_key, data);
    SetManualFallbacks(true);

    std::vector<base::string16> elements = {
        l10n_util::GetStringUTF16(
            IDS_AUTOFILL_PASSWORD_FIELD_SUGGESTIONS_TITLE),
        test_username_};
    if (!IsPreLollipopAndroid() || !custom_passphrase_enabled) {
#if !defined(OS_ANDROID)
      elements.push_back(base::string16());
#endif
      elements.push_back(
          l10n_util::GetStringUTF16(IDS_AUTOFILL_SHOW_ALL_SAVED_FALLBACK));
      if (!custom_passphrase_enabled) {
#if !defined(OS_ANDROID)
        elements.push_back(base::string16());
#endif
        elements.push_back(
            l10n_util::GetStringUTF16(IDS_AUTOFILL_GENERATE_PASSWORD_FALLBACK));
      }
    }

    EXPECT_CALL(
        *autofill_client,
        ShowAutofillPopup(
            element_bounds, _,
            SuggestionVectorValuesAre(testing::ElementsAreArray(elements)), _));

    password_autofill_manager_->OnShowPasswordSuggestions(
        dummy_key, base::i18n::RIGHT_TO_LEFT, test_username_,
        autofill::IS_PASSWORD_FIELD, element_bounds);
  }
