GURL DecorateFrontendURL(const GURL& base_url) {
  std::string frontend_url = base_url.spec();
  std::string url_string(
      frontend_url +
      ((frontend_url.find("?") == std::string::npos) ? "?" : "&") +
      "dockSide=undocked");  
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kEnableDevToolsExperiments))
    url_string += "&experiments=true";

  if (command_line->HasSwitch(switches::kDevToolsFlags)) {
    url_string += "&" + command_line->GetSwitchValueASCII(
        switches::kDevToolsFlags);
  }

#if BUILDFLAG(DEBUG_DEVTOOLS)
  url_string += "&debugFrontend=true";
#endif   

  return GURL(url_string);
}
