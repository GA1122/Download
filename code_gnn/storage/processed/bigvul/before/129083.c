void ExtensionDevToolsClientHost::ReplacedWithAnotherClient() {
  detach_reason_ = OnDetach::REASON_REPLACED_WITH_DEVTOOLS;
}
