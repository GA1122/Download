void DevToolsUIBindings::FileSystemRemoved(
    const std::string& file_system_path) {
  base::StringValue file_system_path_value(file_system_path);
  CallClientFunction("DevToolsAPI.fileSystemRemoved",
                     &file_system_path_value, NULL, NULL);
}
