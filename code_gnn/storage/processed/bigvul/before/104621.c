bool Extension::HasAPIPermission(
    const std::string& function_name) const {
  return permission_set()->HasAccessToFunction(function_name);
}
