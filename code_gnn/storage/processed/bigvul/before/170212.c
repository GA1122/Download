void ComponentUpdaterPolicyTest::FinishDefaultPolicy_GroupPolicySupported() {
  VerifyExpectations(!kUpdateDisabled);

  cur_test_case_ = std::make_pair(
      &ComponentUpdaterPolicyTest::DefaultPolicy_GroupPolicyNotSupported,
      &ComponentUpdaterPolicyTest::FinishDefaultPolicy_GroupPolicyNotSupported);
  CallAsync(cur_test_case_.first);
}
