  void TestStoreInUnloadHandlerForSameSiteNavigation(
      bool preestablish_mojo_pipe) {
    const GURL a_url1 = https_test_server().GetURL("foo.a.com", "/title1.html");
    const GURL a_url2 = https_test_server().GetURL("bar.a.com", "/title2.html");

    ui_test_utils::NavigateToURL(browser(), a_url1);

    ChromePasswordManagerClient* client =
        ChromePasswordManagerClient::FromWebContents(WebContents());

    EXPECT_FALSE(client->has_binding_for_credential_manager());
    if (preestablish_mojo_pipe) {
      ASSERT_NO_FATAL_FAILURE(
          TriggerNavigatorGetPasswordCredentialsAndExpectHasResult(
              WebContents(), false));
      EXPECT_TRUE(client->has_binding_for_credential_manager());
    }

    ASSERT_NO_FATAL_FAILURE(ScheduleNavigatorStoreCredentialAtUnload(
        WebContents(), "user", "hunter2"));

    content::RenderFrameHost* old_rfh = WebContents()->GetMainFrame();
    ui_test_utils::NavigateToURL(browser(), a_url2);
    ASSERT_EQ(old_rfh, WebContents()->GetMainFrame());

    EXPECT_FALSE(client->has_binding_for_credential_manager());

    if (!client->was_store_ever_called())
      return;

    BubbleObserver prompt_observer(WebContents());
    prompt_observer.WaitForAutomaticSavePrompt();
    ASSERT_TRUE(prompt_observer.IsSavePromptShownAutomatically());
    prompt_observer.AcceptSavePrompt();

    WaitForPasswordStore();

    password_manager::TestPasswordStore* test_password_store =
        static_cast<password_manager::TestPasswordStore*>(
            PasswordStoreFactory::GetForProfile(
                browser()->profile(), ServiceAccessType::IMPLICIT_ACCESS)
                .get());

    ASSERT_EQ(1u, test_password_store->stored_passwords().size());
    autofill::PasswordForm signin_form =
        test_password_store->stored_passwords().begin()->second[0];
    EXPECT_EQ(base::ASCIIToUTF16("user"), signin_form.username_value);
    EXPECT_EQ(base::ASCIIToUTF16("hunter2"), signin_form.password_value);
    EXPECT_EQ(a_url1.GetOrigin(), signin_form.origin);
  }
