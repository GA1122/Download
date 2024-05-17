void LocalRTCStatsRequest::requestSucceeded(
    const LocalRTCStatsResponse* response) {
  impl_.RequestSucceeded(response->webKitStatsResponse());
}
