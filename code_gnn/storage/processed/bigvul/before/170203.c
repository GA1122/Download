void ComponentUpdaterPolicyTest::DisabledPolicy_GroupPolicySupported() {
  SetEnableComponentUpdates(false);
  UpdateComponent(MakeCrxComponent(true));
}
