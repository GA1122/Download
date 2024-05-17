void ChildProcessLauncherHelper::SetRegisteredFilesForService(
    const std::string& service_name,
    std::map<std::string, base::FilePath> required_files) {
  SetFilesToShareForServicePosix(service_name, std::move(required_files));
}
