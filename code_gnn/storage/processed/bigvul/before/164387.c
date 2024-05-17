void ExtensionApiTest::SetUpOnMainThread() {
  ExtensionBrowserTest::SetUpOnMainThread();
  DCHECK(!test_config_.get()) << "Previous test did not clear config state.";
  test_config_.reset(new base::DictionaryValue());
  test_config_->SetString(kTestDataDirectory,
                          net::FilePathToFileURL(test_data_dir_).spec());
  if (embedded_test_server()->Started()) {
    test_config_->SetInteger(kEmbeddedTestServerPort,
                             embedded_test_server()->port());
  }
  test_config_->SetBoolean(
      kNativeCrxBindingsEnabled,
      base::FeatureList::IsEnabled(extensions_features::kNativeCrxBindings));
  TestGetConfigFunction::set_test_config_state(test_config_.get());
}
