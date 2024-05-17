ProcessType GetProcessType(const std::wstring& process_type) {
  if (process_type.empty())
    return ProcessType::BROWSER_PROCESS;
  if (process_type == kCloudPrintServiceProcess)
    return ProcessType::CLOUD_PRINT_SERVICE_PROCESS;
#if BUILDFLAG(ENABLE_NACL)
  if (process_type == kNaClBrokerProcess)
    return ProcessType::NACL_BROKER_PROCESS;
  if (process_type == kNaClLoaderProcess)
    return ProcessType::NACL_LOADER_PROCESS;
#endif
  return ProcessType::OTHER_PROCESS;
}
