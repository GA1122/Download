void ApiDefinitionsNatives::GetExtensionAPIDefinitionsForTest(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  std::vector<std::string> apis;
  const FeatureProvider* feature_provider = FeatureProvider::GetAPIFeatures();
  for (const auto& map_entry : feature_provider->GetAllFeatures()) {
    if (!feature_provider->GetParent(map_entry.second.get()) &&
        context()->GetAvailability(map_entry.first).is_available()) {
      apis.push_back(map_entry.first);
    }
  }
  args.GetReturnValue().Set(
      dispatcher_->v8_schema_registry()->GetSchemas(apis));
}
