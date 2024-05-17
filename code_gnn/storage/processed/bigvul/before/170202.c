void ComponentUpdaterPolicyTest::DisabledPolicy_GroupPolicyNotSupported() {
  SetEnableComponentUpdates(false);
  UpdateComponent(MakeCrxComponent(false));
}
