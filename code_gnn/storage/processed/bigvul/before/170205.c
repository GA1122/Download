void ComponentUpdaterPolicyTest::EnabledPolicy_GroupPolicyNotSupported() {
  SetEnableComponentUpdates(true);
  UpdateComponent(MakeCrxComponent(false));
}
