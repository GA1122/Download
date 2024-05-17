void AddPepperFlash(std::vector<content::PepperPluginInfo>* plugins) {
  content::PepperPluginInfo plugin;

  plugin.is_out_of_process = !CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kPpapiFlashInProcess);
  plugin.name = kFlashPluginName;

  std::string flash_version;   

  const CommandLine::StringType flash_path =
      CommandLine::ForCurrentProcess()->GetSwitchValueNative(
          switches::kPpapiFlashPath);
  if (!flash_path.empty()) {
    plugin.path = FilePath(flash_path);

    flash_version = CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
        switches::kPpapiFlashVersion);
  } else {
#if defined(FLAPPER_AVAILABLE) && defined(OS_LINUX) && \
    (defined(ARCH_CPU_X86) || defined(ARCH_CPU_X86_64))
    bool bundled_flapper_enabled = true;
#else
    bool bundled_flapper_enabled = CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kEnableBundledPpapiFlash);
#endif
    bundled_flapper_enabled &= !CommandLine::ForCurrentProcess()->HasSwitch(
                                   switches::kDisableBundledPpapiFlash);
    if (!bundled_flapper_enabled)
      return;

#if defined(FLAPPER_AVAILABLE)
    if (!PathService::Get(chrome::FILE_PEPPER_FLASH_PLUGIN, &plugin.path))
      return;
    flash_version = FLAPPER_VERSION_STRING;
#else
    LOG(ERROR) << "PPAPI Flash not included at build time.";
    return;
#endif   
  }

  std::vector<std::string> flash_version_numbers;
  base::SplitString(flash_version, '.', &flash_version_numbers);
  if (flash_version_numbers.size() < 1)
    flash_version_numbers.push_back("11");
  else if (flash_version_numbers[0].empty())
    flash_version_numbers[0] = "11";
  if (flash_version_numbers.size() < 2)
    flash_version_numbers.push_back("2");
  if (flash_version_numbers.size() < 3)
    flash_version_numbers.push_back("999");
  if (flash_version_numbers.size() < 4)
    flash_version_numbers.push_back("999");
  plugin.description = plugin.name + " " + flash_version_numbers[0] + "." +
      flash_version_numbers[1] + " r" + flash_version_numbers[2];
  plugin.version = JoinString(flash_version_numbers, '.');
  webkit::WebPluginMimeType swf_mime_type(kFlashPluginSwfMimeType,
                                          kFlashPluginSwfExtension,
                                          kFlashPluginSwfDescription);
  plugin.mime_types.push_back(swf_mime_type);
  webkit::WebPluginMimeType spl_mime_type(kFlashPluginSplMimeType,
                                          kFlashPluginSplExtension,
                                          kFlashPluginSplDescription);
  plugin.mime_types.push_back(spl_mime_type);
  plugins->push_back(plugin);
}
