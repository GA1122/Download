NPError NPN_SetValueForURL(NPP id,
                           NPNURLVariable variable,
                           const char* url,
                           const char* value,
                           uint32 len) {
  if (!id)
    return NPERR_INVALID_PARAM;

  if (!url || !*url)
    return NPERR_INVALID_URL;

  switch (variable) {
    case NPNURLVCookie: {
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      if (!plugin)
        return NPERR_GENERIC_ERROR;

      webkit_glue::WebPlugin* webplugin = plugin->webplugin();
      if (!webplugin)
        return NPERR_GENERIC_ERROR;

      std::string cookie(value, len);
      GURL cookies_url((std::string(url)));
      webplugin->SetCookie(cookies_url, cookies_url, cookie);
      return NPERR_NO_ERROR;
    }
    case NPNURLVProxy:
      break;
    default:
      break;
  }

  return NPERR_GENERIC_ERROR;
}
