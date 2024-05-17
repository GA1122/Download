bool DXVAVideoDecodeAccelerator::GetStreamsInfoAndBufferReqs() {
  HRESULT hr = decoder_->GetInputStreamInfo(0, &input_stream_info_);
  RETURN_ON_HR_FAILURE(hr, "Failed to get input stream info", false);

  hr = decoder_->GetOutputStreamInfo(0, &output_stream_info_);
  RETURN_ON_HR_FAILURE(hr, "Failed to get decoder output stream info", false);

  DVLOG(1) << "Input stream info: ";
  DVLOG(1) << "Max latency: " << input_stream_info_.hnsMaxLatency;
  CHECK_EQ(input_stream_info_.dwFlags, 0x7u);

  DVLOG(1) << "Min buffer size: " << input_stream_info_.cbSize;
  DVLOG(1) << "Max lookahead: " << input_stream_info_.cbMaxLookahead;
  DVLOG(1) << "Alignment: " << input_stream_info_.cbAlignment;

  DVLOG(1) << "Output stream info: ";
  DVLOG(1) << "Flags: "
          << std::hex << std::showbase << output_stream_info_.dwFlags;
  CHECK_EQ(output_stream_info_.dwFlags, 0x107u);
  DVLOG(1) << "Min buffer size: " << output_stream_info_.cbSize;
  DVLOG(1) << "Alignment: " << output_stream_info_.cbAlignment;
  return true;
}
