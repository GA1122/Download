void ComponentUpdaterPolicyTest::FinishEnabledPolicy_GroupPolicySupported() {
  VerifyExpectations(!kUpdateDisabled);

  cur_test_case_ = std::make_pair(
      &ComponentUpdaterPolicyTest::EnabledPolicy_GroupPolicyNotSupported,
      &ComponentUpdaterPolicyTest::FinishEnabledPolicy_GroupPolicyNotSupported);
  CallAsync(cur_test_case_.first);
}
