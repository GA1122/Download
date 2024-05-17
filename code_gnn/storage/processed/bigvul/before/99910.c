NPError NPN_GetValueForURL(NPP id,
                           NPNURLVariable variable,
                           const char* url,
                           char** value,
                           uint32* len) {
  if (!id)
    return NPERR_INVALID_PARAM;

  if (!url || !*url || !len)
    return NPERR_INVALID_URL;

  *len = 0;
  std::string result;

  switch (variable) {
    case NPNURLVProxy: {
      result = "DIRECT";
      if (!webkit_glue::FindProxyForUrl(GURL((std::string(url))), &result))
        return NPERR_GENERIC_ERROR;

      break;
    }
    case NPNURLVCookie: {
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      if (!plugin)
        return NPERR_GENERIC_ERROR;

      webkit_glue::WebPlugin* webplugin = plugin->webplugin();
      if (!webplugin)
        return NPERR_GENERIC_ERROR;

      GURL cookies_url((std::string(url)));
      result = webplugin->GetCookies(cookies_url, cookies_url);
      break;
    }
    default:
      return NPERR_GENERIC_ERROR;
  }

  *value = static_cast<char*>(NPN_MemAlloc(result.length() + 1));
  strncpy(*value, result.c_str(), result.length() + 1);
  *len = result.length();

  return NPERR_NO_ERROR;
}
