void PluginChannel::OnClearSiteData(const std::string& site,
                                    uint64 flags,
                                    uint64 max_age) {
  bool success = false;
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  FilePath path = command_line->GetSwitchValuePath(switches::kPluginPath);
  scoped_refptr<webkit::npapi::PluginLib> plugin_lib(
      webkit::npapi::PluginLib::CreatePluginLib(path));
  if (plugin_lib.get()) {
    NPError err = plugin_lib->NP_Initialize();
    if (err == NPERR_NO_ERROR) {
      const char* site_str = site.empty() ? NULL : site.c_str();
      err = plugin_lib->NP_ClearSiteData(site_str, flags, max_age);
      std::string site_name =
          site.empty() ? "NULL"
                       : base::StringPrintf("\"%s\"", site_str);
      VLOG(1) << "NPP_ClearSiteData(" << site_name << ", " << flags << ", "
              << max_age << ") returned " << err;
      success = (err == NPERR_NO_ERROR);
    }
  }
  Send(new PluginHostMsg_ClearSiteDataResult(success));
}
