  MHTMLHandleWriterDelegate(
      mojom::SerializeAsMHTMLParams& params,
      MHTMLHandleWriter::MHTMLWriteCompleteCallback callback,
      scoped_refptr<base::TaskRunner> main_thread_task_runner) {
    DCHECK(params.output_handle);

    if (params.output_handle->is_file_handle()) {
      handle_ = new MHTMLFileHandleWriter(
          std::move(main_thread_task_runner), std::move(callback),
          std::move(params.output_handle->get_file_handle()));
    } else {
      handle_ = new MHTMLProducerHandleWriter(
          std::move(main_thread_task_runner), std::move(callback),
          std::move(params.output_handle->get_producer_handle()));
    }
  }
