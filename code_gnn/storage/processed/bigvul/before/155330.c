base::string16 ChromeContentBrowserClient::GetAppContainerSidForSandboxType(
    int sandbox_type) const {
  switch (sandbox_type) {
    case service_manager::SANDBOX_TYPE_RENDERER:
      return base::string16(install_static::GetSandboxSidPrefix()) +
             L"129201922";
    case service_manager::SANDBOX_TYPE_UTILITY:
      return base::string16();
    case service_manager::SANDBOX_TYPE_GPU:
      return base::string16();
    case service_manager::SANDBOX_TYPE_PPAPI:
      return base::string16(install_static::GetSandboxSidPrefix()) +
             L"129201925";
#if BUILDFLAG(ENABLE_NACL)
    case PROCESS_TYPE_NACL_LOADER:
      return base::string16();
    case PROCESS_TYPE_NACL_BROKER:
      return base::string16();
#endif
  }

  CHECK(0);
  return base::string16();
}
