  ServiceWorkerHandler::ServiceWorkerHandler()
      : DevToolsDomainHandler(ServiceWorker::Metainfo::domainName),
        enabled_(false),
      process_(nullptr),
//       browser_context_(nullptr),
//       storage_partition_(nullptr),
        weak_factory_(this) {}