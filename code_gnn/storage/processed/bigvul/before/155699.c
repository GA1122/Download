 void TestControllerInputRegisteredImpl(WebXrVrBrowserTestBase* t) {
   WebXrControllerInputMock my_mock;
 
  unsigned int controller_index =
      my_mock.CreateAndConnectMinimalGamepad(t->GetPrimaryAxisType());

  t->LoadUrlAndAwaitInitialization(
      t->GetFileUrlForHtmlTestFile("test_webxr_input"));
  t->EnterSessionWithUserGestureOrFail();

  unsigned int num_iterations = 10;
  t->RunJavaScriptOrFail("stepSetupListeners(" +
                         base::NumberToString(num_iterations) + ")");

  for (unsigned int i = 0; i < num_iterations; ++i) {
    my_mock.PressReleasePrimaryTrigger(controller_index);
    t->WaitOnJavaScriptStep();
  }
  t->EndTest();
}
