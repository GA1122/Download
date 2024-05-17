void FileReaderLoader::Start(scoped_refptr<BlobDataHandle> blob_data) {
#if DCHECK_IS_ON()
  DCHECK(!started_loading_) << "FileReaderLoader can only be used once";
  started_loading_ = true;
#endif   

  mojo::ScopedDataPipeProducerHandle producer_handle;
  MojoResult result =
      CreateDataPipe(nullptr, &producer_handle, &consumer_handle_);
  if (result != MOJO_RESULT_OK) {
    Failed(FileError::kNotReadableErr);
    return;
  }

  mojom::blink::BlobReaderClientPtr client_ptr;
  binding_.Bind(MakeRequest(&client_ptr));
  blob_data->ReadAll(std::move(producer_handle), std::move(client_ptr));

  if (IsSyncLoad()) {
    binding_.WaitForIncomingMethodCall();
    if (received_on_complete_)
      return;
    if (!received_all_data_) {
      Failed(FileError::kNotReadableErr);
      return;
    }

    binding_.WaitForIncomingMethodCall();
    if (!received_on_complete_)
      Failed(FileError::kNotReadableErr);
  }
}
