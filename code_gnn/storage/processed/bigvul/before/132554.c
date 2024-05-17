void ShellContentBrowserClient::PreSpawnRenderer(sandbox::TargetPolicy* policy,
                                                 bool* success) {
  std::vector<std::string> font_files = GetSideloadFontFiles();
  for (std::vector<std::string>::const_iterator i(font_files.begin());
      i != font_files.end();
      ++i) {
    policy->AddRule(sandbox::TargetPolicy::SUBSYS_FILES,
        sandbox::TargetPolicy::FILES_ALLOW_READONLY,
        base::UTF8ToWide(*i).c_str());
  }
}
