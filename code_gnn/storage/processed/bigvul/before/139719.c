const ResourceLoaderOptions& ResourceFetcher::DefaultResourceOptions() {
  DEFINE_STATIC_LOCAL(
      ResourceLoaderOptions, options,
      (kBufferData, kAllowStoredCredentials, kClientRequestedCredentials,
       kCheckContentSecurityPolicy, kDocumentContext));
  return options;
}
