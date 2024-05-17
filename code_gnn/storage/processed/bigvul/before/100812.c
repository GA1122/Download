int MapHttpResponseCode(int code) {
  if (HISTOGRAM_MIN_HTTP_RESPONSE_CODE <= code &&
      code <= HISTOGRAM_MAX_HTTP_RESPONSE_CODE)
    return code;
   return 0;
 }
