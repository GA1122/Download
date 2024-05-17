void ComponentUpdaterPolicyTest::UpdateComponent(
    const update_client::CrxComponent& crx_component) {
  post_interceptor_->Reset();
  EXPECT_TRUE(post_interceptor_->ExpectRequest(
      std::make_unique<update_client::PartialMatch>("updatecheck")));
  EXPECT_TRUE(cus_->RegisterComponent(crx_component));
  cus_->GetOnDemandUpdater().OnDemandUpdate(
      component_id_, component_updater::OnDemandUpdater::Priority::FOREGROUND,
      base::BindOnce(&ComponentUpdaterPolicyTest::OnDemandComplete,
                     base::Unretained(this)));
}
