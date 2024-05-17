  LoadingPredictorBrowserTest() {
    scoped_feature_list_.InitWithFeatures(
        {features::kLoadingOnlyLearnHighPriorityResources,
         features::kLoadingPreconnectToRedirectTarget},
        {});
  }
