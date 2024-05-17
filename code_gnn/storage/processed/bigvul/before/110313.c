bool Plugin::Init(uint32_t argc, const char* argn[], const char* argv[]) {
  PLUGIN_PRINTF(("Plugin::Init (argc=%"NACL_PRIu32")\n", argc));
  HistogramEnumerateOsArch(GetSandboxISA());
  init_time_ = NaClGetTimeOfDayMicroseconds();

  ScriptablePlugin* scriptable_plugin = ScriptablePlugin::NewPlugin(this);
  if (scriptable_plugin == NULL)
    return false;

  set_scriptable_plugin(scriptable_plugin);
  PLUGIN_PRINTF(("Plugin::Init (scriptable_handle=%p)\n",
                 static_cast<void*>(scriptable_plugin_)));
  url_util_ = pp::URLUtil_Dev::Get();
  if (url_util_ == NULL)
    return false;

  PLUGIN_PRINTF(("Plugin::Init (url_util_=%p)\n",
                 static_cast<const void*>(url_util_)));

  bool status = Plugin::Init(
      static_cast<int>(argc),
      const_cast<char**>(argn),
      const_cast<char**>(argv));
  if (status) {
    const char* dev_settings = LookupArgument(kDevAttribute);
    enable_dev_interfaces_ = (dev_settings != NULL);

    const char* type_attr = LookupArgument(kTypeAttribute);
    if (type_attr != NULL) {
      mime_type_ = nacl::string(type_attr);
      std::transform(mime_type_.begin(), mime_type_.end(), mime_type_.begin(),
                     tolower);
    }

    const char* manifest_url = LookupArgument(kSrcManifestAttribute);
    if (NexeIsContentHandler()) {
      manifest_url = LookupArgument(kNaClManifestAttribute);
    }
    CHECK(url_util_ != NULL);
    pp::Var base_var = url_util_->GetDocumentURL(this);
    if (!base_var.is_string()) {
      PLUGIN_PRINTF(("Plugin::Init (unable to find document url)\n"));
      return false;
    }
    set_plugin_base_url(base_var.AsString());
    if (manifest_url == NULL) {
      PLUGIN_PRINTF(("Plugin::Init:"
                     " WARNING: no 'src' property, so no manifest loaded.\n"));
      if (NULL != LookupArgument(kNaClManifestAttribute)) {
        PLUGIN_PRINTF(("Plugin::Init:"
                       " WARNING: 'nacl' property is incorrect. Use 'src'.\n"));
      }
    } else {
      RequestNaClManifest(manifest_url);
    }
  }

  PLUGIN_PRINTF(("Plugin::Init (status=%d)\n", status));
  return status;
}
