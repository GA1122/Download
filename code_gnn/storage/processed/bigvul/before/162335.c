    media::BitstreamBuffer>::Read(media::mojom::BitstreamBufferDataView input,
                                  media::BitstreamBuffer* output) {
  base::TimeDelta timestamp;
  if (!input.ReadTimestamp(&timestamp))
    return false;

  std::string key_id;
  if (!input.ReadKeyId(&key_id))
    return false;

  std::string iv;
  if (!input.ReadIv(&iv))
    return false;

  std::vector<media::SubsampleEntry> subsamples;
  if (!input.ReadSubsamples(&subsamples))
    return false;

  mojo::ScopedSharedBufferHandle handle = input.TakeMemoryHandle();
  if (!handle.is_valid())
    return false;

  base::SharedMemoryHandle memory_handle;
  MojoResult unwrap_result = mojo::UnwrapSharedMemoryHandle(
      std::move(handle), &memory_handle, nullptr, nullptr);
  if (unwrap_result != MOJO_RESULT_OK)
    return false;

  media::BitstreamBuffer bitstream_buffer(
      input.id(), memory_handle, input.size(),
      base::checked_cast<off_t>(input.offset()), timestamp);
  if (key_id.size()) {
    bitstream_buffer.SetDecryptConfig(
        media::DecryptConfig(key_id, iv, subsamples));
  }
  *output = bitstream_buffer;

  return true;
}
