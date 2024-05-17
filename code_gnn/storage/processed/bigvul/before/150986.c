void DevToolsUIBindings::FilePathsChanged(
    const std::vector<std::string>& changed_paths,
    const std::vector<std::string>& added_paths,
    const std::vector<std::string>& removed_paths) {
  base::ListValue list;
  for (auto path : changed_paths)
    list.AppendString(path);
  for (auto path : added_paths)
    list.AppendString(path);
  for (auto path : removed_paths)
    list.AppendString(path);

  CallClientFunction("DevToolsAPI.fileSystemFilesChanged",
                     &list, NULL, NULL);
}
