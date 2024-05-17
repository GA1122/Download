bool ParseAuthMethods(
    const std::string& auth_methods_str,
    std::vector<protocol::AuthenticationMethod>* auth_methods) {
  std::vector<std::string> parts;
  base::SplitString(auth_methods_str, ',', &parts);
  for (std::vector<std::string>::iterator it = parts.begin();
       it != parts.end(); ++it) {
    protocol::AuthenticationMethod authentication_method =
        protocol::AuthenticationMethod::FromString(*it);
    if (authentication_method.is_valid())
      auth_methods->push_back(authentication_method);
  }
  if (auth_methods->empty()) {
    LOG(ERROR) << "No valid authentication methods specified.";
    return false;
  }

  return true;
}
