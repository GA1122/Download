std::string TestURLLoader::TestPrefetchBufferThreshold() {
  int32_t rv = OpenWithPrefetchBufferThreshold(-1, 1);
  if (rv != PP_ERROR_FAILED) {
    return ReportError("The prefetch limits contained a negative value but "
                       "the URLLoader did not fail.", rv);
  }

  rv = OpenWithPrefetchBufferThreshold(0, 1);
  if (rv != PP_OK) {
    return ReportError("The prefetch buffer limits were legal values but "
                       "the URLLoader failed.", rv);
  }

  rv = OpenWithPrefetchBufferThreshold(1000, 1);
  if (rv != PP_ERROR_FAILED) {
    return ReportError("The lower buffer value was higher than the upper but "
                       "the URLLoader did not fail.", rv);
  }

  PASS();
}
