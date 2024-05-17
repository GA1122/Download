bool PluginSelectionPolicy::InitFromFile(const FilePath& policy_file) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));

  string data;
  if (!file_util::ReadFileToString(policy_file, &data)) {
    LOG(ERROR) << "Unable to read plugin policy file \""
               << policy_file.value() << "\".";
    init_from_file_finished_ = true;
    return false;
  }

  std::istringstream input_stream(data);
  string line;
  map<string, Policy> policies;
  Policy policy;
  string last_plugin;

  while (std::getline(input_stream, line)) {
    string::size_type pos = line.find("#");
    if (pos != string::npos) {
      line = line.substr(0, pos);
    }
    TrimWhitespaceASCII(line, TRIM_ALL, &line);
    if (line.find("allow") == 0) {
      if (last_plugin.empty()) {
        LOG(ERROR) << "Plugin policy file error: 'allow' out of context.";
        init_from_file_finished_ = true;
        return false;
      }
      line = line.substr(5);
      TrimWhitespaceASCII(line, TRIM_ALL, &line);
      line = StringToLowerASCII(line);
      policy.push_back(make_pair(true, line));
    }
    if (line.find("deny") == 0) {
      if (last_plugin.empty()) {
        LOG(ERROR) << "Plugin policy file error: 'deny' out of context.";
        init_from_file_finished_ = true;
        return false;
      }
      line = line.substr(4);
      TrimWhitespaceASCII(line, TRIM_ALL, &line);
      line = StringToLowerASCII(line);
      policy.push_back(make_pair(false, line));
    }
    if (line.find("plugin") == 0) {
      line = line.substr(6);
      TrimWhitespaceASCII(line, TRIM_ALL, &line);
      if (!policy.empty() && !last_plugin.empty())
        policies.insert(make_pair(last_plugin, policy));
      last_plugin = line;
      policy.clear();
    }
  }

  if (!last_plugin.empty())
    policies.insert(make_pair(last_plugin, policy));

  policies_.swap(policies);
  init_from_file_finished_ = true;
  return true;
}
