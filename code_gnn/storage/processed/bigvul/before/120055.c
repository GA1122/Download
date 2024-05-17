GURL GetFileManagerUrl(const char* path) {
  return GURL(std::string("chrome-extension://") + kFileManagerAppId + path);
}
