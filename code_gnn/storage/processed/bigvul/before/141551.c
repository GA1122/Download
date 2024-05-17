TracingControllerImpl::GenerateMetadataDict() const {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  auto metadata_dict = std::make_unique<base::DictionaryValue>();

  if (trace_config_) {
    DCHECK(IsTracing());
    metadata_dict->SetString("trace-config", trace_config_->ToString());
  }

  metadata_dict->SetString("network-type", GetNetworkTypeString());
  metadata_dict->SetString("product-version",
                           GetContentClient()->browser()->GetProduct());
  metadata_dict->SetString("v8-version", V8_VERSION_STRING);
  metadata_dict->SetString("user-agent",
                           GetContentClient()->browser()->GetUserAgent());

#if defined(OS_ANDROID)
  base::Optional<std::string> soname =
      base::debug::ReadElfLibraryName(&__ehdr_start);
  if (soname)
    metadata_dict->SetString("chrome-library-name", soname.value());
#endif   
  metadata_dict->SetInteger("chrome-bitness", 8 * sizeof(uintptr_t));

#if defined(OS_CHROMEOS)
  metadata_dict->SetString("os-name", "CrOS");
#else
  metadata_dict->SetString("os-name", base::SysInfo::OperatingSystemName());
#endif
  metadata_dict->SetString("os-version",
                           base::SysInfo::OperatingSystemVersion());
#if defined(OS_WIN)
  if (base::win::OSInfo::GetInstance()->architecture() ==
      base::win::OSInfo::X64_ARCHITECTURE) {
    if (base::win::OSInfo::GetInstance()->wow64_status() ==
        base::win::OSInfo::WOW64_ENABLED) {
      metadata_dict->SetString("os-wow64", "enabled");
    } else {
      metadata_dict->SetString("os-wow64", "disabled");
    }
  }

  metadata_dict->SetString("os-session",
                           IsCurrentSessionRemote() ? "remote" : "local");
#endif

  metadata_dict->SetString("os-arch",
                           base::SysInfo::OperatingSystemArchitecture());

  base::CPU cpu;
  metadata_dict->SetInteger("cpu-family", cpu.family());
  metadata_dict->SetInteger("cpu-model", cpu.model());
  metadata_dict->SetInteger("cpu-stepping", cpu.stepping());
  metadata_dict->SetInteger("num-cpus", base::SysInfo::NumberOfProcessors());
  metadata_dict->SetInteger("physical-memory",
                            base::SysInfo::AmountOfPhysicalMemoryMB());

  metadata_dict->SetString("cpu-brand", cpu.cpu_brand());

  const gpu::GPUInfo gpu_info =
      content::GpuDataManagerImpl::GetInstance()->GetGPUInfo();
  const gpu::GPUInfo::GPUDevice& active_gpu = gpu_info.active_gpu();

#if !defined(OS_ANDROID)
  metadata_dict->SetInteger("gpu-venid", active_gpu.vendor_id);
  metadata_dict->SetInteger("gpu-devid", active_gpu.device_id);
#endif

  metadata_dict->SetString("gpu-driver", active_gpu.driver_version);
  metadata_dict->SetString("gpu-psver", gpu_info.pixel_shader_version);
  metadata_dict->SetString("gpu-vsver", gpu_info.vertex_shader_version);

#if defined(OS_MACOSX)
  metadata_dict->SetString("gpu-glver", gpu_info.gl_version);
#elif defined(OS_POSIX)
  metadata_dict->SetString("gpu-gl-vendor", gpu_info.gl_vendor);
  metadata_dict->SetString("gpu-gl-renderer", gpu_info.gl_renderer);
#endif

  metadata_dict->SetString("clock-domain", GetClockString());
  metadata_dict->SetBoolean("highres-ticks",
                            base::TimeTicks::IsHighResolution());

  metadata_dict->SetString(
      "command_line",
      base::CommandLine::ForCurrentProcess()->GetCommandLineString());

  base::Time::Exploded ctime;
  TRACE_TIME_NOW().UTCExplode(&ctime);
  std::string time_string = base::StringPrintf(
      "%u-%u-%u %d:%d:%d", ctime.year, ctime.month, ctime.day_of_month,
      ctime.hour, ctime.minute, ctime.second);
  metadata_dict->SetString("trace-capture-datetime", time_string);

  MetadataFilterPredicate metadata_filter;
  if (trace_config_ && trace_config_->IsArgumentFilterEnabled()) {
    if (delegate_)
      metadata_filter = delegate_->GetMetadataFilterPredicate();
  }

  if (!metadata_filter.is_null()) {
    for (base::DictionaryValue::Iterator it(*metadata_dict); !it.IsAtEnd();
         it.Advance()) {
      if (!metadata_filter.Run(it.key())) {
        metadata_dict->SetString(it.key(), "__stripped__");
      }
    }
  }

  return metadata_dict;
}
