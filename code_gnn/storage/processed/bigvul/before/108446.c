ChromeRenderProcessObserver::ChromeRenderProcessObserver(
    chrome::ChromeContentRendererClient* client)
    : client_(client),
      clear_cache_pending_(false) {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kEnableWatchdog)) {
  }

  if (command_line.HasSwitch(switches::kDumpHistogramsOnExit)) {
    base::StatisticsRecorder::set_dump_on_exit(true);
  }

  RenderThread* thread = RenderThread::Get();
  resource_delegate_.reset(new RendererResourceDelegate());
  thread->SetResourceDispatcherDelegate(resource_delegate_.get());

  net::NetModule::SetResourceProvider(chrome_common_net::NetResourceProvider);

#if defined(OS_WIN)
  FilePath pdf;
  if (PathService::Get(chrome::FILE_PDF_PLUGIN, &pdf) &&
      file_util::PathExists(pdf)) {
    g_iat_patch_createdca.Patch(
        pdf.value().c_str(), "gdi32.dll", "CreateDCA", CreateDCAPatch);
    g_iat_patch_get_font_data.Patch(
        pdf.value().c_str(), "gdi32.dll", "GetFontData", GetFontDataPatch);
  }
#endif

#if defined(OS_POSIX) && !defined(OS_MACOSX) && defined(USE_NSS)
  if (!command_line.HasSwitch(switches::kSingleProcess)) {
    crypto::DisableNSSForkCheck();
    crypto::ForceNSSNoDBInit();
    crypto::EnsureNSSInit();
  }
#elif defined(OS_WIN)
  std::string error;
  base::LoadNativeLibrary(FilePath(L"crypt32.dll"), &error);
#endif
  chrome_variations::SetChildProcessLoggingVariationList();
}
