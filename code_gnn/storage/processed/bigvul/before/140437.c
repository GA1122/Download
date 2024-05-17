  explicit BeforeNetworkStartHandler(bool defer)
      : defer_on_before_network_start_(defer),
        observed_before_network_start_(false) {}
