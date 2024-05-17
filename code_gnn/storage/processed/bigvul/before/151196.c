String ResourcePriorityJSON(ResourceLoadPriority priority) {
  switch (priority) {
    case kResourceLoadPriorityVeryLow:
      return protocol::Network::ResourcePriorityEnum::VeryLow;
    case kResourceLoadPriorityLow:
      return protocol::Network::ResourcePriorityEnum::Low;
    case kResourceLoadPriorityMedium:
      return protocol::Network::ResourcePriorityEnum::Medium;
    case kResourceLoadPriorityHigh:
      return protocol::Network::ResourcePriorityEnum::High;
    case kResourceLoadPriorityVeryHigh:
      return protocol::Network::ResourcePriorityEnum::VeryHigh;
    case kResourceLoadPriorityUnresolved:
      break;
  }
  NOTREACHED();
  return protocol::Network::ResourcePriorityEnum::Medium;
}
