bool IsTemplateParameterString(const std::string& param) {
  return (param.length() > 2) && (*(param.begin()) == kStartParameter) &&
      (*(param.rbegin()) == kEndParameter);
}
