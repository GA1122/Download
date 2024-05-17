bool TemplateURLRef::ParseParameter(size_t start,
                                    size_t end,
                                    std::string* url,
                                    Replacements* replacements) const {
  DCHECK(start != std::string::npos &&
         end != std::string::npos && end > start);
  size_t length = end - start - 1;
  bool optional = false;
  const std::string original_url(*url);
  if (original_url[end - 1] == kOptional) {
    optional = true;
    length--;
  }

  const base::StringPiece parameter(original_url.begin() + start + 1,
                                    original_url.begin() + start + 1 + length);
  const base::StringPiece full_parameter(original_url.begin() + start,
                                         original_url.begin() + end + 1);
  url->erase(start, end - start + 1);
  if (parameter == kSearchTermsParameter) {
    replacements->push_back(Replacement(SEARCH_TERMS, start));
  } else if (parameter == "count") {
    if (!optional)
      url->insert(start, kDefaultCount);
  } else if (parameter == "google:assistedQueryStats") {
    replacements->push_back(Replacement(GOOGLE_ASSISTED_QUERY_STATS, start));
  } else if (parameter == "google:baseURL") {
    replacements->push_back(Replacement(GOOGLE_BASE_URL, start));
  } else if (parameter == "google:baseSuggestURL") {
    replacements->push_back(Replacement(GOOGLE_BASE_SUGGEST_URL, start));
  } else if (parameter == "google:currentPageUrl") {
    replacements->push_back(Replacement(GOOGLE_CURRENT_PAGE_URL, start));
  } else if (parameter == "google:cursorPosition") {
    replacements->push_back(Replacement(GOOGLE_CURSOR_POSITION, start));
  } else if (parameter == "google:forceInstantResults") {
    replacements->push_back(Replacement(GOOGLE_FORCE_INSTANT_RESULTS, start));
  } else if (parameter == "google:imageOriginalHeight") {
    replacements->push_back(
        Replacement(TemplateURLRef::GOOGLE_IMAGE_ORIGINAL_HEIGHT, start));
  } else if (parameter == "google:imageOriginalWidth") {
    replacements->push_back(
        Replacement(TemplateURLRef::GOOGLE_IMAGE_ORIGINAL_WIDTH, start));
  } else if (parameter == "google:imageSearchSource") {
    replacements->push_back(
        Replacement(TemplateURLRef::GOOGLE_IMAGE_SEARCH_SOURCE, start));
  } else if (parameter == "google:imageThumbnail") {
    replacements->push_back(
        Replacement(TemplateURLRef::GOOGLE_IMAGE_THUMBNAIL, start));
  } else if (parameter == "google:imageURL") {
    replacements->push_back(Replacement(TemplateURLRef::GOOGLE_IMAGE_URL,
                                        start));
  } else if (parameter == "google:inputType") {
    replacements->push_back(Replacement(TemplateURLRef::GOOGLE_INPUT_TYPE,
                                        start));
  } else if (parameter == "google:instantExtendedEnabledParameter") {
    replacements->push_back(Replacement(GOOGLE_INSTANT_EXTENDED_ENABLED,
                                        start));
  } else if (parameter == "google:instantExtendedEnabledKey") {
    url->insert(start, google_util::kInstantExtendedAPIParam);
  } else if (parameter == "google:iOSSearchLanguage") {
    replacements->push_back(Replacement(GOOGLE_IOS_SEARCH_LANGUAGE, start));
  } else if (parameter == "google:contextualSearchVersion") {
    replacements->push_back(
        Replacement(GOOGLE_CONTEXTUAL_SEARCH_VERSION, start));
  } else if (parameter == "google:contextualSearchContextData") {
    replacements->push_back(
        Replacement(GOOGLE_CONTEXTUAL_SEARCH_CONTEXT_DATA, start));
  } else if (parameter == "google:originalQueryForSuggestion") {
    replacements->push_back(Replacement(GOOGLE_ORIGINAL_QUERY_FOR_SUGGESTION,
                                        start));
  } else if (parameter == "google:pageClassification") {
    replacements->push_back(Replacement(GOOGLE_PAGE_CLASSIFICATION, start));
  } else if (parameter == "google:prefetchQuery") {
    replacements->push_back(Replacement(GOOGLE_PREFETCH_QUERY, start));
  } else if (parameter == "google:RLZ") {
    replacements->push_back(Replacement(GOOGLE_RLZ, start));
  } else if (parameter == "google:searchClient") {
    replacements->push_back(Replacement(GOOGLE_SEARCH_CLIENT, start));
  } else if (parameter == "google:searchFieldtrialParameter") {
    replacements->push_back(Replacement(GOOGLE_SEARCH_FIELDTRIAL_GROUP, start));
  } else if (parameter == "google:searchVersion") {
    replacements->push_back(Replacement(GOOGLE_SEARCH_VERSION, start));
  } else if (parameter == "google:sessionToken") {
    replacements->push_back(Replacement(GOOGLE_SESSION_TOKEN, start));
  } else if (parameter == "google:sourceId") {
#if defined(OS_ANDROID) || defined(OS_IOS)
    url->insert(start, "sourceid=chrome-mobile&");
#else
    url->insert(start, "sourceid=chrome&");
#endif
  } else if (parameter == "google:suggestAPIKeyParameter") {
    url->insert(start,
                net::EscapeQueryParamValue(google_apis::GetAPIKey(), false));
  } else if (parameter == "google:suggestClient") {
    replacements->push_back(Replacement(GOOGLE_SUGGEST_CLIENT, start));
  } else if (parameter == "google:suggestRid") {
    replacements->push_back(Replacement(GOOGLE_SUGGEST_REQUEST_ID, start));
  } else if (parameter == kGoogleUnescapedSearchTermsParameter) {
    replacements->push_back(Replacement(GOOGLE_UNESCAPED_SEARCH_TERMS, start));
  } else if (parameter == "yandex:searchPath") {
    switch (ui::GetDeviceFormFactor()) {
      case ui::DEVICE_FORM_FACTOR_DESKTOP:
        url->insert(start, "search/");
        break;
      case ui::DEVICE_FORM_FACTOR_PHONE:
        url->insert(start, "search/touch/");
        break;
      case ui::DEVICE_FORM_FACTOR_TABLET:
        url->insert(start, "search/pad/");
        break;
    }
  } else if (parameter == "inputEncoding") {
    replacements->push_back(Replacement(ENCODING, start));
  } else if (parameter == "language") {
    replacements->push_back(Replacement(LANGUAGE, start));
  } else if (parameter == "outputEncoding") {
    if (!optional)
      url->insert(start, kOutputEncodingType);
  } else if ((parameter == "startIndex") || (parameter == "startPage")) {
    if (!optional)
      url->insert(start, "1");
  } else if (!prepopulated_) {
    url->insert(start, full_parameter.data(), full_parameter.size());
    return false;
  }
  return true;
}
