v8::Local<v8::Object> Dispatcher::GetOrCreateBindObjectIfAvailable(
    const std::string& api_name,
    std::string* bind_name,
    ScriptContext* context) {
  std::vector<std::string> split = base::SplitString(
      api_name, ".", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

  v8::Local<v8::Object> bind_object;

  const FeatureProvider* api_feature_provider =
      FeatureProvider::GetAPIFeatures();
  std::string ancestor_name;
  bool only_ancestor_available = false;

  for (size_t i = 0; i < split.size() - 1; ++i) {
    ancestor_name += (i ? "." : "") + split[i];
    if (api_feature_provider->GetFeature(ancestor_name) &&
        context->GetAvailability(ancestor_name).is_available() &&
        !context->GetAvailability(api_name).is_available()) {
      only_ancestor_available = true;
      break;
    }

    if (bind_object.IsEmpty()) {
      bind_object = AsObjectOrEmpty(GetOrCreateChrome(context));
      if (bind_object.IsEmpty())
        return v8::Local<v8::Object>();
    }
    bind_object = GetOrCreateObject(bind_object, split[i], context->isolate());
  }

  if (only_ancestor_available)
    return v8::Local<v8::Object>();

  if (bind_name)
    *bind_name = split.back();

  return bind_object.IsEmpty() ? AsObjectOrEmpty(GetOrCreateChrome(context))
                               : bind_object;
}
