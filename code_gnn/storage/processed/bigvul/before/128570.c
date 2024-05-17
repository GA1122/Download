std::string Instance::GetLocalizedString(PP_ResourceString id) {
  pp::Var rv(pp::PDF::GetLocalizedString(this, id));
  if (!rv.is_string())
    return std::string();

  return rv.AsString();
}
