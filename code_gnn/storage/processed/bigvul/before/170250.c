void ComponentUpdaterPolicyTest::OnDemandComplete(update_client::Error error) {
  CallAsync(cur_test_case_.second);
}
