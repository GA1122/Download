void TestInputSourcesChangeImpl(WebXrVrBrowserTestBase* t) {
  WebXrControllerInputMock my_mock;

  uint64_t insufficient_buttons =
      device::XrButtonMaskFromId(device::XrButtonId::kAxisTrigger);
  std::map<device::XrButtonId, unsigned int> insufficient_axis_types = {
      {device::XrButtonId::kAxisTrigger, device::XrAxisType::kTrigger},
  };
  unsigned int controller_index = my_mock.CreateAndConnectController(
      device::ControllerRole::kControllerRoleRight, insufficient_axis_types,
      insufficient_buttons);

  t->LoadUrlAndAwaitInitialization(
      t->GetFileUrlForHtmlTestFile("test_webxr_input_sources_change_event"));
  t->EnterSessionWithUserGestureOrFail();

  t->PollJavaScriptBooleanOrFail("inputChangeEvents === 1",
                                 WebXrVrBrowserTestBase::kPollTimeoutShort);

  t->RunJavaScriptOrFail("validateAdded(1)");
  t->RunJavaScriptOrFail("validateRemoved(0)");
  t->RunJavaScriptOrFail("updateCachedInputSource(0)");
  t->RunJavaScriptOrFail("validateCachedAddedPresence(true)");

  my_mock.DisconnectController(controller_index);
  t->PollJavaScriptBooleanOrFail("inputChangeEvents === 2",
                                 WebXrVrBrowserTestBase::kPollTimeoutShort);

  t->RunJavaScriptOrFail("validateAdded(0)");
  t->RunJavaScriptOrFail("validateRemoved(1)");
  t->RunJavaScriptOrFail("validateCachedRemovedPresence(true)");

  controller_index =
      my_mock.CreateAndConnectMinimalGamepad(t->GetPrimaryAxisType());
  t->PollJavaScriptBooleanOrFail("inputChangeEvents === 3",
                                 WebXrVrBrowserTestBase::kPollTimeoutShort);
  t->RunJavaScriptOrFail("updateCachedInputSource(0)");

  if (t->GetRuntimeType() != XrBrowserTestBase::RuntimeType::RUNTIME_WMR) {
    my_mock.UpdateControllerSupport(controller_index, insufficient_axis_types,
                                    insufficient_buttons);

    t->PollJavaScriptBooleanOrFail("inputChangeEvents === 4",
                                   WebXrVrBrowserTestBase::kPollTimeoutShort);
    t->RunJavaScriptOrFail("validateAdded(1)");
    t->RunJavaScriptOrFail("validateRemoved(1)");
    t->RunJavaScriptOrFail("validateCachedAddedPresence(false)");
    t->RunJavaScriptOrFail("validateCachedRemovedPresence(true)");
  }

  t->RunJavaScriptOrFail("done()");
  t->EndTest();
}
