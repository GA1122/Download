  LoadingPredictorNetworkIsolationKeyBrowserTest() {
    switch (GetParam()) {
      case NetworkIsolationKeyMode::kNone:
        scoped_feature_list2_.InitWithFeatures(
            {features::kLoadingPreconnectToRedirectTarget},
            {net::features::kPartitionConnectionsByNetworkIsolationKey,
             net::features::kSplitCacheByNetworkIsolationKey,
             net::features::kAppendFrameOriginToNetworkIsolationKey});
        break;
      case NetworkIsolationKeyMode::kTopFrameOrigin:
        scoped_feature_list2_.InitWithFeatures(
            {net::features::kPartitionConnectionsByNetworkIsolationKey,
             net::features::kSplitCacheByNetworkIsolationKey,
             features::kLoadingPreconnectToRedirectTarget},
            {net::features::kAppendFrameOriginToNetworkIsolationKey});
        break;
      case NetworkIsolationKeyMode::kTopFrameAndFrameOrigins:
        scoped_feature_list2_.InitWithFeatures(
            {net::features::kPartitionConnectionsByNetworkIsolationKey,
             net::features::kSplitCacheByNetworkIsolationKey,
             net::features::kAppendFrameOriginToNetworkIsolationKey,
             features::kLoadingPreconnectToRedirectTarget},
            {});
        break;
    }
  }
