bool ContentSecurityPolicy::AllowRequest(
    mojom::RequestContextType context,
    const KURL& url,
    const String& nonce,
    const IntegrityMetadataSet& integrity_metadata,
    ParserDisposition parser_disposition,
    RedirectStatus redirect_status,
    SecurityViolationReportingPolicy reporting_policy,
    CheckHeaderType check_header_type) const {
  if (integrity_metadata.IsEmpty() &&
      !AllowRequestWithoutIntegrity(context, url, redirect_status,
                                    reporting_policy, check_header_type)) {
    return false;
  }

  switch (context) {
    case mojom::RequestContextType::AUDIO:
    case mojom::RequestContextType::TRACK:
    case mojom::RequestContextType::VIDEO:
      return AllowMediaFromSource(url, redirect_status, reporting_policy,
                                  check_header_type);
    case mojom::RequestContextType::BEACON:
    case mojom::RequestContextType::EVENT_SOURCE:
    case mojom::RequestContextType::FETCH:
    case mojom::RequestContextType::PING:
    case mojom::RequestContextType::XML_HTTP_REQUEST:
    case mojom::RequestContextType::SUBRESOURCE:
      return AllowConnectToSource(url, redirect_status, reporting_policy,
                                  check_header_type);
    case mojom::RequestContextType::EMBED:
    case mojom::RequestContextType::OBJECT:
      return AllowObjectFromSource(url, redirect_status, reporting_policy,
                                   check_header_type);
    case mojom::RequestContextType::PREFETCH:
      return AllowPrefetchFromSource(url, redirect_status, reporting_policy,
                                     check_header_type);
    case mojom::RequestContextType::FAVICON:
    case mojom::RequestContextType::IMAGE:
    case mojom::RequestContextType::IMAGE_SET:
      return AllowImageFromSource(url, redirect_status, reporting_policy,
                                  check_header_type);
    case mojom::RequestContextType::FONT:
      return AllowFontFromSource(url, redirect_status, reporting_policy,
                                 check_header_type);
    case mojom::RequestContextType::FORM:
      return AllowFormAction(url, redirect_status, reporting_policy,
                             check_header_type);
    case mojom::RequestContextType::FRAME:
    case mojom::RequestContextType::IFRAME:
      return AllowFrameFromSource(url, redirect_status, reporting_policy,
                                  check_header_type);
    case mojom::RequestContextType::IMPORT:
    case mojom::RequestContextType::SCRIPT:
    case mojom::RequestContextType::XSLT:
      return AllowScriptFromSource(url, nonce, integrity_metadata,
                                   parser_disposition, redirect_status,
                                   reporting_policy, check_header_type);
    case mojom::RequestContextType::MANIFEST:
      return AllowManifestFromSource(url, redirect_status, reporting_policy,
                                     check_header_type);
    case mojom::RequestContextType::SERVICE_WORKER:
    case mojom::RequestContextType::SHARED_WORKER:
    case mojom::RequestContextType::WORKER:
      return AllowWorkerContextFromSource(url, redirect_status,
                                          reporting_policy, check_header_type);
    case mojom::RequestContextType::STYLE:
      return AllowStyleFromSource(url, nonce, redirect_status, reporting_policy,
                                  check_header_type);
    case mojom::RequestContextType::CSP_REPORT:
    case mojom::RequestContextType::DOWNLOAD:
    case mojom::RequestContextType::HYPERLINK:
    case mojom::RequestContextType::INTERNAL:
    case mojom::RequestContextType::LOCATION:
    case mojom::RequestContextType::PLUGIN:
    case mojom::RequestContextType::UNSPECIFIED:
      return true;
  }
  NOTREACHED();
  return true;
}
