void DevToolsUIBindings::RequestFileSystems() {
  CHECK(web_contents_->GetURL().SchemeIs(content::kChromeDevToolsScheme));
  std::vector<DevToolsFileHelper::FileSystem> file_systems =
      file_helper_->GetFileSystems();
  base::ListValue file_systems_value;
  for (size_t i = 0; i < file_systems.size(); ++i)
    file_systems_value.Append(CreateFileSystemValue(file_systems[i]));
  CallClientFunction("DevToolsAPI.fileSystemsLoaded",
                     &file_systems_value, NULL, NULL);
}
