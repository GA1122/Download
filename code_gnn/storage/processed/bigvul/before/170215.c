void ComponentUpdaterPolicyTest::FinishEnabledPolicy_GroupPolicyNotSupported() {
  VerifyExpectations(!kUpdateDisabled);

  cur_test_case_ = std::make_pair(
      &ComponentUpdaterPolicyTest::DisabledPolicy_GroupPolicySupported,
      &ComponentUpdaterPolicyTest::FinishDisabled_PolicyGroupPolicySupported);
  CallAsync(cur_test_case_.first);
}
