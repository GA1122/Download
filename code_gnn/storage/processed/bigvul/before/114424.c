static IMFSample* CreateInputSample(const uint8* stream, int size,
                                    int min_size, int alignment) {
  CHECK(stream);
  CHECK_GT(size, 0);
  base::win::ScopedComPtr<IMFSample> sample;
  sample.Attach(CreateEmptySampleWithBuffer(std::max(min_size, size),
                                            alignment));
  RETURN_ON_FAILURE(sample, "Failed to create empty sample", NULL);

  base::win::ScopedComPtr<IMFMediaBuffer> buffer;
  HRESULT hr = sample->GetBufferByIndex(0, buffer.Receive());
  RETURN_ON_HR_FAILURE(hr, "Failed to get buffer from sample", NULL);

  DWORD max_length = 0;
  DWORD current_length = 0;
  uint8* destination = NULL;
  hr = buffer->Lock(&destination, &max_length, &current_length);
  RETURN_ON_HR_FAILURE(hr, "Failed to lock buffer", NULL);

  CHECK_EQ(current_length, 0u);
  CHECK_GE(static_cast<int>(max_length), size);
  memcpy(destination, stream, size);

  hr = buffer->Unlock();
  RETURN_ON_HR_FAILURE(hr, "Failed to unlock buffer", NULL);

  hr = buffer->SetCurrentLength(size);
  RETURN_ON_HR_FAILURE(hr, "Failed to set buffer length", NULL);

  return sample.Detach();
}
