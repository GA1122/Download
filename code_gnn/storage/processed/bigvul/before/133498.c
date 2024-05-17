void AwMainDelegate::PreSandboxStartup() {
#if defined(ARCH_CPU_ARM_FAMILY)
  base::CPU cpu_info;
#endif

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  std::string process_type =
      command_line.GetSwitchValueASCII(switches::kProcessType);
  if (process_type == switches::kRendererProcess) {
    auto global_descriptors = base::GlobalDescriptors::GetInstance();
    int pak_fd = global_descriptors->Get(kAndroidWebViewLocalePakDescriptor);
    base::MemoryMappedFile::Region pak_region =
        global_descriptors->GetRegion(kAndroidWebViewLocalePakDescriptor);
    ResourceBundle::InitSharedInstanceWithPakFileRegion(base::File(pak_fd),
                                                        pak_region);
    pak_fd = global_descriptors->Get(kAndroidWebViewMainPakDescriptor);
    pak_region =
        global_descriptors->GetRegion(kAndroidWebViewMainPakDescriptor);
    ResourceBundle::GetSharedInstance().AddDataPackFromFileRegion(
        base::File(pak_fd), pak_region, ui::SCALE_FACTOR_NONE);
  }

  crash_reporter::EnableMicrodumpCrashReporter();
}
