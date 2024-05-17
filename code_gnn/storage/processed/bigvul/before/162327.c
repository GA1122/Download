bool MojoVideoEncodeAccelerator::Initialize(VideoPixelFormat input_format,
                                            const gfx::Size& input_visible_size,
                                            VideoCodecProfile output_profile,
                                            uint32_t initial_bitrate,
                                            Client* client) {
  DVLOG(2) << __func__
           << " input_format=" << VideoPixelFormatToString(input_format)
           << ", input_visible_size=" << input_visible_size.ToString()
           << ", output_profile=" << GetProfileName(output_profile)
           << ", initial_bitrate=" << initial_bitrate;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!client)
    return false;

  mojom::VideoEncodeAcceleratorClientPtr vea_client_ptr;
  vea_client_ = std::make_unique<VideoEncodeAcceleratorClient>(
      client, mojo::MakeRequest(&vea_client_ptr));

  bool result = false;
  vea_->Initialize(input_format, input_visible_size, output_profile,
                   initial_bitrate, std::move(vea_client_ptr), &result);
  return result;
}
