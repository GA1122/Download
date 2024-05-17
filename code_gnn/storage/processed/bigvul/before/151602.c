void ResourceLoader::DidDownloadData(int length, int encoded_data_length) {
  Context().DispatchDidDownloadData(resource_->Identifier(), length,
                                    encoded_data_length);
  resource_->DidDownloadData(length);
}
