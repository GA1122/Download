String resourcePriority(net::RequestPriority priority) {
  switch (priority) {
    case net::MINIMUM_PRIORITY:
    case net::IDLE:
      return Network::ResourcePriorityEnum::VeryLow;
    case net::LOWEST:
      return Network::ResourcePriorityEnum::Low;
    case net::LOW:
      return Network::ResourcePriorityEnum::Medium;
    case net::MEDIUM:
      return Network::ResourcePriorityEnum::High;
    case net::HIGHEST:
      return Network::ResourcePriorityEnum::VeryHigh;
  }
  NOTREACHED();
  return Network::ResourcePriorityEnum::Medium;
}
