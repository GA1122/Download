base::FilePath GetAnyChromePath() {
  base::FilePath chrome_path;
#ifndef OFFICIAL_BUILD
  chrome_path = GetDevelopmentExe(kChromeExe);
#endif
  if (chrome_path.empty())
    chrome_path = GetChromePathForInstallationLevel(SYSTEM_LEVEL_INSTALLATION);
  if (chrome_path.empty())
    chrome_path = GetChromePathForInstallationLevel(USER_LEVEL_INSTALLATION);
  return chrome_path;
}
