void ShellContentBrowserClient::ResourceDispatcherHostCreated() {
  resource_dispatcher_host_delegate_.reset(
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kRunLayoutTest)
          ? new LayoutTestResourceDispatcherHostDelegate
          : new ShellResourceDispatcherHostDelegate);
  ResourceDispatcherHost::Get()->SetDelegate(
      resource_dispatcher_host_delegate_.get());
}
