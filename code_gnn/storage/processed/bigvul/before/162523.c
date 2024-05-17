size_t Resource::CalculateOverheadSize() const {
  static const int kAverageClientsHashMapSize = 384;
  return sizeof(Resource) + GetResponse().MemoryUsage() +
         kAverageClientsHashMapSize +
         GetResourceRequest().Url().GetString().length() * 2;
}
