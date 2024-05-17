void RegisterContentSchemes(const char** additional_savable_schemes) {
  url_util::AddStandardScheme(kChromeDevToolsScheme);
  url_util::AddStandardScheme(kChromeUIScheme);
  url_util::AddStandardScheme(kMetadataScheme);

  url_util::LockStandardSchemes();

  if (additional_savable_schemes) {
    int schemes = 0;
    while (additional_savable_schemes[++schemes]);
    g_savable_schemes = new char*[schemes + arraysize(kDefaultSavableSchemes)];
    memcpy(g_savable_schemes,
           kDefaultSavableSchemes,
           arraysize(kDefaultSavableSchemes) * sizeof(char*));
    for (int i = 0; i < schemes; ++i) {
      g_savable_schemes[arraysize(kDefaultSavableSchemes) + i - 1] =
          base::strdup(additional_savable_schemes[i]);
    }
    g_savable_schemes[arraysize(kDefaultSavableSchemes) + schemes - 1] = 0;
  }
}
