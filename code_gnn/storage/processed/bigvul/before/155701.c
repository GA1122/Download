void TestInputGamepadSameObjectImpl(WebXrVrBrowserTestBase* t) {
  WebXrControllerInputMock my_mock;

  uint64_t insufficient_buttons =
      device::XrButtonMaskFromId(device::XrButtonId::kAxisTrigger);
  std::map<device::XrButtonId, unsigned int> insufficient_axis_types = {
      {device::XrButtonId::kAxisTrigger, device::XrAxisType::kTrigger},
  };

  uint64_t sufficient_buttons =
      device::XrButtonMaskFromId(device::XrButtonId::kAxisTrigger) |
      device::XrButtonMaskFromId(device::XrButtonId::kAxisPrimary);
  std::map<device::XrButtonId, unsigned int> sufficient_axis_types = {
      {device::XrButtonId::kAxisPrimary, device::XrAxisType::kTrackpad},
      {device::XrButtonId::kAxisTrigger, device::XrAxisType::kTrigger},
  };

  unsigned int controller_index = my_mock.CreateAndConnectController(
      device::ControllerRole::kControllerRoleRight, insufficient_axis_types,
      insufficient_buttons);

  t->LoadUrlAndAwaitInitialization(
      t->GetFileUrlForHtmlTestFile("test_webxr_input_same_object"));
  t->EnterSessionWithUserGestureOrFail();

  t->PollJavaScriptBooleanOrFail("inputChangeEvents === 1",
                                 WebXrVrBrowserTestBase::kPollTimeoutShort);

  t->RunJavaScriptOrFail("validateInputSourceLength(1)");
  t->RunJavaScriptOrFail("updateCachedInputSource(0)");

  my_mock.PressReleasePrimaryTrigger(controller_index);
  t->RunJavaScriptOrFail("validateCachedSourcePresence(true)");
  t->RunJavaScriptOrFail("validateCurrentAndCachedGamepadMatch()");

  my_mock.UpdateControllerSupport(controller_index, sufficient_axis_types,
                                  sufficient_buttons);
  t->PollJavaScriptBooleanOrFail("inputChangeEvents === 2",
                                 WebXrVrBrowserTestBase::kPollTimeoutShort);
  t->RunJavaScriptOrFail("validateCachedSourcePresence(false)");
  t->RunJavaScriptOrFail("validateInputSourceLength(1)");
  t->RunJavaScriptOrFail("updateCachedInputSource(0)");

  my_mock.PressReleasePrimaryTrigger(controller_index);
  t->RunJavaScriptOrFail("validateCachedSourcePresence(true)");
  t->RunJavaScriptOrFail("validateCurrentAndCachedGamepadMatch()");

  my_mock.UpdateControllerSupport(controller_index, insufficient_axis_types,
                                  insufficient_buttons);
  t->PollJavaScriptBooleanOrFail("inputChangeEvents === 3",
                                 WebXrVrBrowserTestBase::kPollTimeoutShort);
  t->RunJavaScriptOrFail("validateCachedSourcePresence(false)");
  t->RunJavaScriptOrFail("validateInputSourceLength(1)");
  t->RunJavaScriptOrFail("done()");
  t->EndTest();
}
