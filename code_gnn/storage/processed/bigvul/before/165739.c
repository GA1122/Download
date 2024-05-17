const std::string& GetVariationParam(
    const std::map<std::string, std::string>& params,
    const std::string& key) {
  auto it = params.find(key);
  if (it == params.end())
    return base::EmptyString();

  return it->second;
}
