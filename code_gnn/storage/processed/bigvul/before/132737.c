bool ChromotingInstance::IsCallerAppOrExtension() {
  const pp::URLUtil_Dev* url_util = pp::URLUtil_Dev::Get();
  if (!url_util)
    return false;

  PP_URLComponents_Dev url_components;
  pp::Var url_var = url_util->GetDocumentURL(this, &url_components);
  if (!url_var.is_string())
    return false;

  std::string url = url_var.AsString();
  std::string url_scheme = url.substr(url_components.scheme.begin,
                                      url_components.scheme.len);
  return url_scheme == kChromeExtensionUrlScheme;
}
