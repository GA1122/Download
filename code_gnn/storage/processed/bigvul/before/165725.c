net::URLRequestContextBuilder::HttpCacheParams::Type ChooseCacheType() {
#if !defined(OS_ANDROID)
  const std::string experiment_name =
      base::FieldTrialList::FindFullName("SimpleCacheTrial");
  if (base::StartsWith(experiment_name, "Disable",
                       base::CompareCase::INSENSITIVE_ASCII)) {
    return net::URLRequestContextBuilder::HttpCacheParams::DISK_BLOCKFILE;
  }

#if defined(OS_MACOSX) && !defined(OS_IOS)
  if (base::mac::IsAtLeastOS10_14())
    return net::URLRequestContextBuilder::HttpCacheParams::DISK_SIMPLE;
#endif   

  if (base::StartsWith(experiment_name, "ExperimentYes",
                       base::CompareCase::INSENSITIVE_ASCII)) {
    return net::URLRequestContextBuilder::HttpCacheParams::DISK_SIMPLE;
  }
#endif   

#if defined(OS_ANDROID) || defined(OS_LINUX) || defined(OS_CHROMEOS)
  return net::URLRequestContextBuilder::HttpCacheParams::DISK_SIMPLE;
#else
  return net::URLRequestContextBuilder::HttpCacheParams::DISK_BLOCKFILE;
#endif
}
