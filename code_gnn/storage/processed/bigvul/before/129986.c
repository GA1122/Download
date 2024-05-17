void LaunchDevToolsHandlerIfNeeded(const base::CommandLine& command_line) {
  if (command_line.HasSwitch(::switches::kRemoteDebuggingPort)) {
    std::string port_str =
        command_line.GetSwitchValueASCII(::switches::kRemoteDebuggingPort);
    int port;
    if (base::StringToInt(port_str, &port) && port >= 0 && port < 65535) {
      g_browser_process->CreateDevToolsHttpProtocolHandler(
          chrome::HOST_DESKTOP_TYPE_NATIVE,
          "127.0.0.1",
          static_cast<uint16>(port));
    } else {
      DLOG(WARNING) << "Invalid http debugger port number " << port;
    }
  }
}
