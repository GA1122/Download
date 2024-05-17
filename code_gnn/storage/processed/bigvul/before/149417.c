bool ContentSecurityPolicy::allowRequest(
    WebURLRequest::RequestContext context,
    const KURL& url,
    const String& nonce,
    const IntegrityMetadataSet& integrityMetadata,
    ParserDisposition parserDisposition,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  if (integrityMetadata.isEmpty() &&
      !allowRequestWithoutIntegrity(context, url, redirectStatus,
                                    reportingPolicy))
    return false;

  switch (context) {
    case WebURLRequest::RequestContextAudio:
    case WebURLRequest::RequestContextTrack:
    case WebURLRequest::RequestContextVideo:
      return allowMediaFromSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextBeacon:
    case WebURLRequest::RequestContextEventSource:
    case WebURLRequest::RequestContextFetch:
    case WebURLRequest::RequestContextXMLHttpRequest:
    case WebURLRequest::RequestContextSubresource:
      return allowConnectToSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextEmbed:
    case WebURLRequest::RequestContextObject:
      return allowObjectFromSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextFavicon:
    case WebURLRequest::RequestContextImage:
    case WebURLRequest::RequestContextImageSet:
      return allowImageFromSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextFont:
      return allowFontFromSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextForm:
      return allowFormAction(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextFrame:
    case WebURLRequest::RequestContextIframe:
      return allowFrameFromSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextImport:
    case WebURLRequest::RequestContextScript:
      return allowScriptFromSource(url, nonce, parserDisposition,
                                   redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextXSLT:
      return allowScriptFromSource(url, nonce, parserDisposition,
                                   redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextManifest:
      return allowManifestFromSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextServiceWorker:
    case WebURLRequest::RequestContextSharedWorker:
    case WebURLRequest::RequestContextWorker:
      return allowWorkerContextFromSource(url, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextStyle:
      return allowStyleFromSource(url, nonce, redirectStatus, reportingPolicy);
    case WebURLRequest::RequestContextCSPReport:
    case WebURLRequest::RequestContextDownload:
    case WebURLRequest::RequestContextHyperlink:
    case WebURLRequest::RequestContextInternal:
    case WebURLRequest::RequestContextLocation:
    case WebURLRequest::RequestContextPing:
    case WebURLRequest::RequestContextPlugin:
    case WebURLRequest::RequestContextPrefetch:
    case WebURLRequest::RequestContextUnspecified:
      return true;
  }
  ASSERT_NOT_REACHED();
  return true;
}
