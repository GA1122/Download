void RecordDownloadHttpResponseCode(int response_code) {
  UMA_HISTOGRAM_CUSTOM_ENUMERATION(
      "Download.HttpResponseCode",
      net::HttpUtil::MapStatusCodeForHistogram(response_code),
      net::HttpUtil::GetStatusCodesForHistogram());
}
