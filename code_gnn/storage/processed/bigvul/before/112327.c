net::RequestPriority DetermineRequestPriority(ResourceType::Type type) {

  switch (type) {
    case ResourceType::MAIN_FRAME:
    case ResourceType::SUB_FRAME:
      return net::HIGHEST;

    case ResourceType::STYLESHEET:
    case ResourceType::SCRIPT:
    case ResourceType::FONT_RESOURCE:
      return net::MEDIUM;

    case ResourceType::SUB_RESOURCE:
    case ResourceType::OBJECT:
    case ResourceType::MEDIA:
    case ResourceType::WORKER:
    case ResourceType::SHARED_WORKER:
    case ResourceType::XHR:
      return net::LOW;

    case ResourceType::IMAGE:
    case ResourceType::FAVICON:
      return net::LOWEST;

    case ResourceType::PREFETCH:
    case ResourceType::PRERENDER:
      return net::IDLE;

    default:
      NOTREACHED();
      return net::LOW;
  }
}
