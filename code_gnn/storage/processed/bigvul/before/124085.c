content::BrowserMainParts* ChromeContentBrowserClient::CreateBrowserMainParts(
    const content::MainFunctionParams& parameters) {
  ChromeBrowserMainParts* main_parts;
#if defined(OS_WIN)
  main_parts = new ChromeBrowserMainPartsWin(parameters);
#elif defined(OS_MACOSX)
  main_parts = new ChromeBrowserMainPartsMac(parameters);
#elif defined(OS_CHROMEOS)
  main_parts = new chromeos::ChromeBrowserMainPartsChromeos(parameters);
#elif defined(OS_LINUX)
  main_parts = new ChromeBrowserMainPartsLinux(parameters);
#elif defined(OS_ANDROID)
  main_parts = new ChromeBrowserMainPartsAndroid(parameters);
#elif defined(OS_POSIX)
  main_parts = new ChromeBrowserMainPartsPosix(parameters);
#else
  NOTREACHED();
  main_parts = new ChromeBrowserMainParts(parameters);
#endif

#if defined(TOOLKIT_GTK)
  chrome::AddGtkToolkitExtraParts(main_parts);
#endif

#if defined(TOOLKIT_VIEWS)
  chrome::AddViewsToolkitExtraParts(main_parts);
#endif

#if defined(USE_ASH)
  chrome::AddAshToolkitExtraParts(main_parts);
#endif

#if defined(USE_AURA)
  chrome::AddAuraToolkitExtraParts(main_parts);
#endif

  return main_parts;
}
