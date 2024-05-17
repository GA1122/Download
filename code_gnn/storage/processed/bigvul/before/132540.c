BrowserMainParts* ShellContentBrowserClient::CreateBrowserMainParts(
    const MainFunctionParams& parameters) {
  shell_browser_main_parts_ = base::CommandLine::ForCurrentProcess()->HasSwitch(
                                  switches::kRunLayoutTest)
                                  ? new LayoutTestBrowserMainParts(parameters)
                                  : new ShellBrowserMainParts(parameters);
  return shell_browser_main_parts_;
}
