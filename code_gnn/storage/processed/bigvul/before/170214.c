void ComponentUpdaterPolicyTest::FinishDisabled_PolicyGroupPolicySupported() {
  VerifyExpectations(kUpdateDisabled);

  cur_test_case_ = std::make_pair(
      &ComponentUpdaterPolicyTest::DisabledPolicy_GroupPolicyNotSupported,
      &ComponentUpdaterPolicyTest::
          FinishDisabledPolicy_GroupPolicyNotSupported);
  CallAsync(cur_test_case_.first);
}
