bool ChromeContentRendererClient::IsNaClAllowed(
    const GURL& manifest_url,
    const GURL& top_url,
    bool is_nacl_unrestricted,
    bool is_extension_unrestricted,
    bool is_extension_from_webstore,
    WebPluginParams* params) {
  bool is_whitelisted_url =
      ((top_url.SchemeIs("http") || top_url.SchemeIs("https")) &&
      (top_url.host() == "plus.google.com" ||
          top_url.host() == "plus.sandbox.google.com") &&
      top_url.path().find("/games") == 0);

  bool is_nacl_allowed =
      is_extension_from_webstore ||
      is_extension_unrestricted ||
      is_whitelisted_url ||
      is_nacl_unrestricted;
  if (is_nacl_allowed) {
    bool app_can_use_dev_interfaces =
        (is_extension_from_webstore &&
        manifest_url.SchemeIs("chrome-extension") &&
        manifest_url.host() == "acadkphlmlegjaadjagenfimbpphcgnh");
    WebString dev_attribute = WebString::fromUTF8("@dev");
    if ((!is_whitelisted_url && !is_extension_from_webstore) ||
        app_can_use_dev_interfaces) {
      std::vector<string16> param_names;
      std::vector<string16> param_values;
      param_names.push_back(dev_attribute);
      param_values.push_back(WebString());
      AppendParams(
          param_names,
          param_values,
          &params->attributeNames,
          &params->attributeValues);
    } else {
      size_t attribute_count = params->attributeNames.size();
      for (size_t i = 0; i < attribute_count; ++i) {
        if (params->attributeNames[i].equals(dev_attribute))
          params->attributeNames[i] = WebString();
      }
    }
  }
  return is_nacl_allowed;
}
