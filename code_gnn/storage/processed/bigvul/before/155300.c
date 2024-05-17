content::BrowserMainParts* ChromeContentBrowserClient::CreateBrowserMainParts(
    const content::MainFunctionParams& parameters) {
  ChromeBrowserMainParts* main_parts;
#if defined(OS_WIN)
  main_parts =
      new ChromeBrowserMainPartsWin(parameters, chrome_feature_list_creator_);
#elif defined(OS_MACOSX)
  main_parts =
      new ChromeBrowserMainPartsMac(parameters, chrome_feature_list_creator_);
#elif defined(OS_CHROMEOS)
  main_parts = new chromeos::ChromeBrowserMainPartsChromeos(
      parameters, chrome_feature_list_creator_);
#elif defined(OS_LINUX)
  main_parts =
      new ChromeBrowserMainPartsLinux(parameters, chrome_feature_list_creator_);
#elif defined(OS_ANDROID)
  main_parts = new ChromeBrowserMainPartsAndroid(parameters,
                                                 chrome_feature_list_creator_);
#elif defined(OS_POSIX)
  main_parts =
      new ChromeBrowserMainPartsPosix(parameters, chrome_feature_list_creator_);
#else
  NOTREACHED();
  main_parts =
      new ChromeBrowserMainParts(parameters, chrome_feature_list_creator_);
#endif

  chrome::AddProfilesExtraParts(main_parts);

#if defined(TOOLKIT_VIEWS)
#if defined(OS_LINUX) && !defined(OS_CHROMEOS) && !defined(USE_OZONE)
  main_parts->AddParts(new ChromeBrowserMainExtraPartsViewsLinux());
#else
  main_parts->AddParts(new ChromeBrowserMainExtraPartsViews());
#endif
#endif

#if defined(OS_CHROMEOS)
  main_parts->AddParts(new ChromeBrowserMainExtraPartsAsh());
#endif

#if defined(USE_X11)
  main_parts->AddParts(new ChromeBrowserMainExtraPartsX11());
#endif

  main_parts->AddParts(new ChromeBrowserMainExtraPartsResourceCoordinator);

  main_parts->AddParts(new ChromeBrowserMainExtraPartsProfiling);

  chrome::AddMetricsExtraParts(main_parts);

  main_parts->AddParts(ChromeService::GetInstance()->CreateExtraParts());

  return main_parts;
}
