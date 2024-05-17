  bool HasHostBeenLookedUp(const std::string& host) {
    return base::Contains(successful_dns_lookups_, host) ||
           base::Contains(unsuccessful_dns_lookups_, host);
  }
