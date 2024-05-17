void MetricsLog::RecordCoreSystemProfile(MetricsServiceClient* client,
                                         SystemProfileProto* system_profile) {
  system_profile->set_build_timestamp(metrics::MetricsLog::GetBuildTime());
  system_profile->set_app_version(client->GetVersionString());
  system_profile->set_channel(client->GetChannel());
  system_profile->set_application_locale(client->GetApplicationLocale());

#if defined(ADDRESS_SANITIZER) || DCHECK_IS_ON()
  system_profile->set_is_instrumented_build(true);
#endif

  metrics::SystemProfileProto::Hardware* hardware =
      system_profile->mutable_hardware();
#if !defined(OS_IOS)
  hardware->set_cpu_architecture(base::SysInfo::OperatingSystemArchitecture());
#endif
  hardware->set_system_ram_mb(base::SysInfo::AmountOfPhysicalMemoryMB());
  hardware->set_hardware_class(base::SysInfo::HardwareModelName());
#if defined(OS_WIN)
  hardware->set_dll_base(reinterpret_cast<uint64_t>(CURRENT_MODULE()));
#endif

  metrics::SystemProfileProto::OS* os = system_profile->mutable_os();
  os->set_name(base::SysInfo::OperatingSystemName());
  os->set_version(base::SysInfo::OperatingSystemVersion());

#if defined(OS_CHROMEOS)
  os->set_kernel_version(base::SysInfo::KernelVersion());
#elif defined(OS_LINUX)
  os->set_kernel_version(base::SysInfo::OperatingSystemVersion());
#endif

#if defined(OS_ANDROID)
  os->set_build_fingerprint(
      base::android::BuildInfo::GetInstance()->android_build_fp());
  std::string package_name = client->GetAppPackageName();
  if (!package_name.empty() && package_name != "com.android.chrome")
    system_profile->set_app_package_name(package_name);
#endif
}
