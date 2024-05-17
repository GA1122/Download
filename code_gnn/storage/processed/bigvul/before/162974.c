void ManifestUmaUtil::FetchSucceeded() {
  UMA_HISTOGRAM_ENUMERATION(kUMANameFetchResult,
                            MANIFEST_FETCH_SUCCESS,
                            MANIFEST_FETCH_RESULT_TYPE_COUNT);
}
