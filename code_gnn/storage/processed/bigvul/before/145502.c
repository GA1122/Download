bool IsDetachableResourceType(ResourceType type) {
  switch (type) {
    case RESOURCE_TYPE_PREFETCH:
    case RESOURCE_TYPE_PING:
    case RESOURCE_TYPE_CSP_REPORT:
      return true;
    default:
      return false;
  }
}
