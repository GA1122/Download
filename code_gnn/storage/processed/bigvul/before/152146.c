bool ServiceWorkerContextCore::IsValidRegisterRequest(
    const GURL& script_url,
    const GURL& scope_url,
    std::string* out_error) const {
  if (!scope_url.is_valid() || !script_url.is_valid()) {
    *out_error = ServiceWorkerConsts::kBadMessageInvalidURL;
    return false;
  }
  if (ServiceWorkerUtils::ContainsDisallowedCharacter(scope_url, script_url,
                                                      out_error)) {
    return false;
  }
  std::vector<GURL> urls = {scope_url, script_url};
  if (!ServiceWorkerUtils::AllOriginsMatchAndCanAccessServiceWorkers(urls)) {
    *out_error = ServiceWorkerConsts::kBadMessageImproperOrigins;
    return false;
  }
  return true;
}
