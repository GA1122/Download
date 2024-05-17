FileReaderLoader::~FileReaderLoader() {
  Cleanup();
  UnadjustReportedMemoryUsageToV8();
}
