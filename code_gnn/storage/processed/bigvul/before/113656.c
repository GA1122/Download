PPB_URLLoader_Impl::PPB_URLLoader_Impl(PP_Instance instance,
                                       bool main_document_loader)
    : Resource(::ppapi::OBJECT_IS_IMPL, instance),
      main_document_loader_(main_document_loader),
      pending_callback_(),
      bytes_sent_(0),
      total_bytes_to_be_sent_(-1),
      bytes_received_(0),
      total_bytes_to_be_received_(-1),
      user_buffer_(NULL),
      user_buffer_size_(0),
      done_status_(PP_OK_COMPLETIONPENDING),
      is_streaming_to_file_(false),
      is_asynchronous_load_suspended_(false),
      has_universal_access_(false),
      status_callback_(NULL) {
}
