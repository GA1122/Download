bool Plugin::SetManifestObject(const nacl::string& manifest_json,
                               ErrorInfo* error_info) {
  PLUGIN_PRINTF(("Plugin::SetManifestObject(): manifest_json='%s'.\n",
       manifest_json.c_str()));
  if (error_info == NULL)
    return false;
  bool should_prefer_portable =
      (getenv("NACL_PREFER_PORTABLE_IN_MANIFEST") != NULL);
  nacl::scoped_ptr<JsonManifest> json_manifest(
      new JsonManifest(url_util_,
                       manifest_base_url(),
                       GetSandboxISA(),
                       should_prefer_portable));
  if (!json_manifest->Init(manifest_json, error_info)) {
    return false;
  }
  manifest_.reset(json_manifest.release());
  return true;
}
