InputHandler::InputHandler()
    : DevToolsDomainHandler(Input::Metainfo::domainName),
      host_(nullptr),
      input_queued_(false),
      page_scale_factor_(1.0),
      last_id_(0),
      weak_factory_(this) {}
