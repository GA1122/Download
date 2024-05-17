void Core::SetMachPortProvider(base::PortProvider* port_provider) {
#if defined(OS_MACOSX) && !defined(OS_IOS)
  GetNodeController()->CreateMachPortRelay(port_provider);
#endif
}
