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

  AutoReset<bool> reentrancy_protector(&in_data_received_, true);
  ProcessData(data, length);

  const char* segment;
  size_t pos = 0;
  while (size_t length = data_buffer_->GetSomeData(segment, pos)) {
    ProcessData(segment, length);
    pos += length;
  }
  data_buffer_->Clear();
}
