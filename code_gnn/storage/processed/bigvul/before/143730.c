ContentBrowserClient* ShellMainDelegate::CreateContentBrowserClient() {
  browser_client_.reset(switches::IsRunWebTestsSwitchPresent()
                            ? new LayoutTestContentBrowserClient
                            : new ShellContentBrowserClient);

  return browser_client_.get();
}
