void DocumentLoader::DataReceived(Resource* resource,
                                  const char* data,
                                  size_t length) {
  DCHECK(data);
  DCHECK(length);
  DCHECK_EQ(resource, GetResource());
  DCHECK(!response_.IsNull());
  DCHECK(!frame_->GetPage()->Paused());

  if (in_data_received_) {
    data_buffer_->Append(data, length);
    return;
  }

  base::AutoReset<bool> reentrancy_protector(&in_data_received_, true);
  ProcessData(data, length);
  ProcessDataBuffer();
}
