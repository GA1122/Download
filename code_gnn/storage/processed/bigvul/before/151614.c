static bool IsManualRedirectFetchRequest(const ResourceRequest& request) {
  return request.GetFetchRedirectMode() ==
             WebURLRequest::kFetchRedirectModeManual &&
         request.GetRequestContext() == WebURLRequest::kRequestContextFetch;
}
