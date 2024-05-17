    SRIVerifier(std::unique_ptr<WebDataConsumerHandle> handle,
                SRIBytesConsumer* updater,
                Response* response,
                FetchManager::Loader* loader,
                String integrity_metadata,
                const KURL& url,
                FetchResponseType response_type,
                scoped_refptr<base::SingleThreadTaskRunner> task_runner)
        : handle_(std::move(handle)),
          updater_(updater),
          response_(response),
          loader_(loader),
          integrity_metadata_(integrity_metadata),
          url_(url),
          response_type_(response_type),
          finished_(false) {
      reader_ = handle_->ObtainReader(this, std::move(task_runner));
    }
