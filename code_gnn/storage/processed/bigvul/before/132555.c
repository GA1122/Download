void ShellContentBrowserClient::RenderProcessWillLaunch(
    RenderProcessHost* host) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kExposeIpcEcho))
    host->AddFilter(new IPCEchoMessageFilter());
}
