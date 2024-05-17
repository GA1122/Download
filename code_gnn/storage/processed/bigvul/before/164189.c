    explicit MockStorageDelegate(AppCacheStorageImplTest* test)
        : loaded_cache_id_(0),
          stored_group_success_(false),
          would_exceed_quota_(false),
          obsoleted_success_(false),
          found_cache_id_(blink::mojom::kAppCacheNoCacheId),
          test_(test) {}
