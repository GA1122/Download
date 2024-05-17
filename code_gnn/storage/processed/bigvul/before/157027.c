int64_t MultibufferDataSource::GetMemoryUsage() const {
  return base::checked_cast<int64_t>(url_data()->CachedSize())
         << url_data()->multibuffer()->block_size_shift();
}
