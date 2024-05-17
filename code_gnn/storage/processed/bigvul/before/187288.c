 NetworkThrottleManagerImpl::NetworkThrottleManagerImpl()
      : lifetime_median_estimate_(PercentileEstimator::kMedianPercentile,
                                  kInitialMedianInMs),
        outstanding_recomputation_timer_(
          base::MakeUnique<base::Timer>(false  ,
//           std::make_unique<base::Timer>(false  ,
                                          false  )),
        tick_clock_(new base::DefaultTickClock()),
        weak_ptr_factory_(this) {}