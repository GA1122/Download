void ExtensionApiTest::SetCustomArg(base::StringPiece custom_arg) {
  test_config_->SetKey(kTestCustomArg, base::Value(custom_arg));
}
