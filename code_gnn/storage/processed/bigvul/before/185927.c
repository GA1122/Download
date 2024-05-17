 std::string DataUseUserData::GetServiceNameAsString(ServiceName service_name) {
   switch (service_name) {
     case SUGGESTIONS:
       return "Suggestions";
     case NOT_TAGGED:
       return "NotTagged";
     case TRANSLATE:
       return "Translate";
     case SYNC:
       return "Sync";
     case OMNIBOX:
       return "Omnibox";
     case INVALIDATION:
       return "Invalidation";
     case RAPPOR:
       return "Rappor";
     case VARIATIONS:
       return "Variations";
     case UMA:
       return "UMA";
     case DOMAIN_RELIABILITY:
       return "DomainReliability";
     case PROFILE_DOWNLOADER:
       return "ProfileDownloader";
     case GOOGLE_URL_TRACKER:
       return "GoogleURLTracker";
     case AUTOFILL:
       return "Autofill";
     case POLICY:
       return "Policy";
     case SPELL_CHECKER:
       return "SpellChecker";
     case NTP_SNIPPETS:
       return "NTPSnippets";
     case SAFE_BROWSING:
       return "SafeBrowsing";
     case DATA_REDUCTION_PROXY:
       return "DataReductionProxy";
     case PRECACHE:
       return "Precache";
     case NTP_TILES:
       return "NTPTiles";
     case FEEDBACK_UPLOADER:
       return "FeedbackUploader";
     case TRACING_UPLOADER:
       return "TracingUploader";
     case DOM_DISTILLER:
       return "DOMDistiller";
     case CLOUD_PRINT:
       return "CloudPrint";
     case SEARCH_PROVIDER_LOGOS:
       return "SearchProviderLogos";
     case UPDATE_CLIENT:
       return "UpdateClient";
     case GCM_DRIVER:
       return "GCMDriver";
     case WEB_HISTORY_SERVICE:
       return "WebHistoryService";
     case NETWORK_TIME_TRACKER:
       return "NetworkTimeTracker";
     case SUPERVISED_USER:
       return "SupervisedUser";
     case IMAGE_FETCHER_UNTAGGED:
        return "ImageFetcherUntagged";
      case GAIA:
        return "GAIA";
//     case CAPTIVE_PORTAL:
//       return "CaptivePortal";
//     case WEB_RESOURCE_SERVICE:
//       return "WebResourceService";
//     case SIGNIN:
//       return "Signin";
    }
    return "INVALID";
  }