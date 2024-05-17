std::string AddSafeSearchParameters(const std::string& query) {
  std::vector<std::string> new_parameters;
  std::string safe_parameter = chrome::kSafeSearchSafeParameter;
  std::string ssui_parameter = chrome::kSafeSearchSsuiParameter;

  std::vector<std::string> parameters;
  base::SplitString(query, '&', &parameters);

  std::vector<std::string>::iterator it;
  for (it = parameters.begin(); it < parameters.end(); ++it) {
    if (!HasSameParameterKey(*it, safe_parameter) &&
        !HasSameParameterKey(*it, ssui_parameter)) {
      new_parameters.push_back(*it);
    }
  }

  new_parameters.push_back(safe_parameter);
  new_parameters.push_back(ssui_parameter);
  return JoinString(new_parameters, '&');
}
