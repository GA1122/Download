void ComponentUpdaterPolicyTest::EnabledPolicy_GroupPolicySupported() {
  SetEnableComponentUpdates(true);
  UpdateComponent(MakeCrxComponent(true));
}
