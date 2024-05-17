std::unique_ptr<NavigationThrottle> TargetHandler::CreateThrottleForNavigation(
    NavigationHandle* navigation_handle) {
  if (!auto_attacher_.ShouldThrottleFramesNavigation())
    return nullptr;
  return std::make_unique<Throttle>(weak_factory_.GetWeakPtr(),
                                    navigation_handle);
}
