BlobURLRequestJob::BlobURLRequestJob(
    net::URLRequest* request,
    net::NetworkDelegate* network_delegate,
    BlobData* blob_data,
    fileapi::FileSystemContext* file_system_context,
    base::MessageLoopProxy* file_thread_proxy)
    : net::URLRequestJob(request, network_delegate),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_factory_(this)),
      blob_data_(blob_data),
      file_system_context_(file_system_context),
      file_thread_proxy_(file_thread_proxy),
      total_size_(0),
      remaining_bytes_(0),
      pending_get_file_info_count_(0),
      current_item_index_(0),
      current_item_offset_(0),
      error_(false),
      headers_set_(false),
      byte_range_set_(false) {
  DCHECK(file_thread_proxy_);
}
