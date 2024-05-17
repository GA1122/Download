const char* NPN_UserAgent(NPP id) {
#if defined(OS_WIN)
  if (id) {
    scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
    if (plugin.get() && !plugin->use_mozilla_user_agent())
      return webkit_glue::GetUserAgent(GURL()).c_str();
  }

  return "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9a1) Gecko/20061103 Firefox/2.0a1";
#else
  return webkit_glue::GetUserAgent(GURL()).c_str();
#endif
}
