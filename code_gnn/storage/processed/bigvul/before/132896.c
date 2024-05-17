  ResourceProvider::ResourceId IncrementResourceId(
      ResourceProvider::ResourceId id) {
    ++num_resources_;
    return id + 1;
  }
