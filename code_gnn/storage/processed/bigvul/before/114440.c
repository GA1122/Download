bool DXVAVideoDecodeAccelerator::ProcessOutputSample(IMFSample* sample) {
  RETURN_ON_FAILURE(sample, "Decode succeeded with NULL output sample", false);

  base::win::ScopedComPtr<IMFSample> output_sample;
  output_sample.Attach(sample);

  base::win::ScopedComPtr<IMFMediaBuffer> output_buffer;
  HRESULT hr = sample->GetBufferByIndex(0, output_buffer.Receive());
  RETURN_ON_HR_FAILURE(hr, "Failed to get buffer from output sample", false);

  base::win::ScopedComPtr<IDirect3DSurface9> surface;
  hr = MFGetService(output_buffer, MR_BUFFER_SERVICE,
                    IID_PPV_ARGS(surface.Receive()));
  RETURN_ON_HR_FAILURE(hr, "Failed to get D3D surface from output sample",
                       false);

  D3DSURFACE_DESC surface_desc;
  hr = surface->GetDesc(&surface_desc);
  RETURN_ON_HR_FAILURE(hr, "Failed to get surface description", false);

  pending_output_samples_.push_back(
      PendingSampleInfo(last_input_buffer_id_, surface));

  if (output_picture_buffers_.size()) {
    ProcessPendingSamples();
    return true;
  }
  if (pictures_requested_) {
    DVLOG(1) << "Waiting for picture slots from the client.";
    return true;
  }
  MessageLoop::current()->PostTask(FROM_HERE, base::Bind(
      &DXVAVideoDecodeAccelerator::RequestPictureBuffers,
      this, surface_desc.Width, surface_desc.Height));

  pictures_requested_ = true;
  return true;
}
