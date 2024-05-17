bool HasSameParameterKey(const std::string& first_parameter,
                         const std::string& second_parameter) {
  DCHECK(second_parameter.find("=") != std::string::npos);
  std::string parameter_prefix = second_parameter.substr(
      0, second_parameter.find("=") + 1);
  return StartsWithASCII(first_parameter, parameter_prefix, false);
}
