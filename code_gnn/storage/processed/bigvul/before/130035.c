  bool StoreSeedForTesting(const std::string& seed_data) {
    return StoreSeedData(seed_data, std::string(), std::string(),
                         base::Time::Now(), false, nullptr);
  }
