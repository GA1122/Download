void MediaRecorder::WriteData(const char* data,
                              size_t length,
                              bool last_in_slice,
                              double timecode) {
  if (stopped_ && !last_in_slice) {
    stopped_ = false;
    ScheduleDispatchEvent(Event::Create(event_type_names::kStart));
  }

  if (!blob_data_) {
    blob_data_ = BlobData::Create();
    blob_data_->SetContentType(mime_type_);
  }
  if (data)
    blob_data_->AppendBytes(data, length);

  if (!last_in_slice)
    return;

  const long long blob_data_length = blob_data_->length();
  CreateBlobEvent(Blob::Create(BlobDataHandle::Create(std::move(blob_data_),
                                                      blob_data_length)),
                  timecode);
}
