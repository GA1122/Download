void WebLocalFrameImpl::UsageCountChromeLoadTimes(const WebString& metric) {
  WebFeature feature = WebFeature::kChromeLoadTimesUnknown;
  if (metric == "requestTime") {
    feature = WebFeature::kChromeLoadTimesRequestTime;
  } else if (metric == "startLoadTime") {
    feature = WebFeature::kChromeLoadTimesStartLoadTime;
  } else if (metric == "commitLoadTime") {
    feature = WebFeature::kChromeLoadTimesCommitLoadTime;
  } else if (metric == "finishDocumentLoadTime") {
    feature = WebFeature::kChromeLoadTimesFinishDocumentLoadTime;
  } else if (metric == "finishLoadTime") {
    feature = WebFeature::kChromeLoadTimesFinishLoadTime;
  } else if (metric == "firstPaintTime") {
    feature = WebFeature::kChromeLoadTimesFirstPaintTime;
  } else if (metric == "firstPaintAfterLoadTime") {
    feature = WebFeature::kChromeLoadTimesFirstPaintAfterLoadTime;
  } else if (metric == "navigationType") {
    feature = WebFeature::kChromeLoadTimesNavigationType;
  } else if (metric == "wasFetchedViaSpdy") {
    feature = WebFeature::kChromeLoadTimesWasFetchedViaSpdy;
  } else if (metric == "wasNpnNegotiated") {
    feature = WebFeature::kChromeLoadTimesWasNpnNegotiated;
  } else if (metric == "npnNegotiatedProtocol") {
    feature = WebFeature::kChromeLoadTimesNpnNegotiatedProtocol;
  } else if (metric == "wasAlternateProtocolAvailable") {
    feature = WebFeature::kChromeLoadTimesWasAlternateProtocolAvailable;
  } else if (metric == "connectionInfo") {
    feature = WebFeature::kChromeLoadTimesConnectionInfo;
  }
  Deprecation::CountDeprecation(GetFrame(), feature);
}
