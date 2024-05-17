    FinishDisabledPolicy_GroupPolicyNotSupported() {
  VerifyExpectations(!kUpdateDisabled);

  cur_test_case_ = TestCase();
  CallAsync(&ComponentUpdaterPolicyTest::EndTest);
}
