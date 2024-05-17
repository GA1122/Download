bool IsEvictableError(AppCacheUpdateJob::ResultType result,
                      const AppCacheErrorDetails& details) {
  switch (result) {
    case AppCacheUpdateJob::DB_ERROR:
    case AppCacheUpdateJob::DISKCACHE_ERROR:
    case AppCacheUpdateJob::QUOTA_ERROR:
    case AppCacheUpdateJob::NETWORK_ERROR:
    case AppCacheUpdateJob::CANCELLED_ERROR:
      return false;

    case AppCacheUpdateJob::REDIRECT_ERROR:
    case AppCacheUpdateJob::SERVER_ERROR:
    case AppCacheUpdateJob::SECURITY_ERROR:
      return true;

    case AppCacheUpdateJob::MANIFEST_ERROR:
      return details.reason == APPCACHE_SIGNATURE_ERROR;

    default:
      NOTREACHED();
      return true;
  }
}
