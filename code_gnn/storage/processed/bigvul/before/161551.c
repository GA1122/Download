void GoogleChromeDistribution::DoPostUninstallOperations(
    const base::Version& version,
    const base::FilePath& local_data_path,
    const base::string16& distribution_data) {
  const base::string16 kVersionParam = L"crversion";
  const base::string16 kOSParam = L"os";

  const base::win::OSInfo* os_info = base::win::OSInfo::GetInstance();
  base::win::OSInfo::VersionNumber version_number = os_info->version_number();
  base::string16 os_version =
      base::StringPrintf(L"%d.%d.%d", version_number.major,
                         version_number.minor, version_number.build);

  base::string16 url = GetUninstallSurveyUrl() + L"&" + kVersionParam + L"=" +
                       base::ASCIIToUTF16(version.GetString()) + L"&" +
                       kOSParam + L"=" + os_version;

  base::string16 uninstall_metrics;
  if (installer::ExtractUninstallMetricsFromFile(local_data_path,
                                                 &uninstall_metrics)) {
    url += uninstall_metrics;
    if (!distribution_data.empty()) {
      url += L"&";
      url += distribution_data;
    }
  }

  if (os_info->version() >= base::win::VERSION_WIN10 &&
      NavigateToUrlWithEdge(url)) {
    return;
  }
  NavigateToUrlWithIExplore(url);
}
