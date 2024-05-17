void ChromeContentBrowserClient::GetAdditionalMappedFilesForChildProcess(
    const CommandLine& command_line,
    int child_process_id,
    std::vector<FileDescriptorInfo>* mappings) {
#if defined(OS_ANDROID)
  base::FilePath data_path;
  PathService::Get(ui::DIR_RESOURCE_PAKS_ANDROID, &data_path);
  DCHECK(!data_path.empty());

  int flags = base::PLATFORM_FILE_OPEN | base::PLATFORM_FILE_READ;
  base::FilePath chrome_pak = data_path.AppendASCII("chrome.pak");
  base::PlatformFile f =
      base::CreatePlatformFile(chrome_pak, flags, NULL, NULL);
  DCHECK(f != base::kInvalidPlatformFileValue);
  mappings->push_back(FileDescriptorInfo(kAndroidChromePakDescriptor,
                                         FileDescriptor(f, true)));

  base::FilePath chrome_resources_pak =
      data_path.AppendASCII("chrome_100_percent.pak");
  f = base::CreatePlatformFile(chrome_resources_pak, flags, NULL, NULL);
  DCHECK(f != base::kInvalidPlatformFileValue);
  mappings->push_back(FileDescriptorInfo(kAndroidChrome100PercentPakDescriptor,
                                         FileDescriptor(f, true)));

  const std::string locale = GetApplicationLocale();
  base::FilePath locale_pak = ResourceBundle::GetSharedInstance().
      GetLocaleFilePath(locale, false);
  f = base::CreatePlatformFile(locale_pak, flags, NULL, NULL);
  DCHECK(f != base::kInvalidPlatformFileValue);
  mappings->push_back(FileDescriptorInfo(kAndroidLocalePakDescriptor,
                                         FileDescriptor(f, true)));

  base::FilePath resources_pack_path;
  PathService::Get(chrome::FILE_RESOURCES_PACK, &resources_pack_path);
  f = base::CreatePlatformFile(resources_pack_path, flags, NULL, NULL);
  DCHECK(f != base::kInvalidPlatformFileValue);
  mappings->push_back(FileDescriptorInfo(kAndroidUIResourcesPakDescriptor,
                                         FileDescriptor(f, true)));

#if defined(USE_LINUX_BREAKPAD)
  if (IsCrashReporterEnabled()) {
    f = CrashDumpManager::GetInstance()->CreateMinidumpFile(child_process_id);
    if (f == base::kInvalidPlatformFileValue) {
      LOG(ERROR) << "Failed to create file for minidump, crash reporting will "
                 "be disabled for this process.";
    } else {
      mappings->push_back(FileDescriptorInfo(kAndroidMinidumpDescriptor,
                                             FileDescriptor(f, true)));
    }
  }
#endif   

#else
  int crash_signal_fd = GetCrashSignalFD(command_line);
  if (crash_signal_fd >= 0) {
    mappings->push_back(FileDescriptorInfo(kCrashDumpSignal,
                                           FileDescriptor(crash_signal_fd,
                                                          false)));
  }
#endif   
}
