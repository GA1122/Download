MemBackendImpl::MemBackendImpl(net::NetLog* net_log)
    : max_size_(0), current_size_(0), net_log_(net_log), weak_factory_(this) {
}
