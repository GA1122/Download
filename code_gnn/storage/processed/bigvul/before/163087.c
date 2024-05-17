BlobStorageContext::BlobStorageContext()
    : memory_controller_(base::FilePath(), scoped_refptr<base::TaskRunner>()),
      ptr_factory_(this) {}
