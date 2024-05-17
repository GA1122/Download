void DevToolsUIBindings::SearchCompleted(
    int request_id,
    const std::string& file_system_path,
    const std::vector<std::string>& file_paths) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  base::ListValue file_paths_value;
  for (std::vector<std::string>::const_iterator it(file_paths.begin());
       it != file_paths.end(); ++it) {
    file_paths_value.AppendString(*it);
  }
  base::FundamentalValue request_id_value(request_id);
  base::StringValue file_system_path_value(file_system_path);
  CallClientFunction("DevToolsAPI.searchCompleted", &request_id_value,
                     &file_system_path_value, &file_paths_value);
}
