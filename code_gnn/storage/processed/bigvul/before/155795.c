void RecordGaiaSignoutMetrics(ChromePrimaryAccountStateInGaiaCookies state) {
  UMA_HISTOGRAM_ENUMERATION(kChromePrimaryAccountStateOnWebSignoutHistogram,
                            state,
                            kChromePrimaryAccountStateInGaiaCookiesCount);
}
