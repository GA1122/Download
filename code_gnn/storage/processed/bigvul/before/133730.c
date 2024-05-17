  bool RunSearchEnginesSubtest(const std::string& subtest) {
    return RunExtensionSubtest("search_engines_private",
                               "main.html?" + subtest,
                               kFlagLoadAsComponent);
  }
