bool DXVAVideoDecodeAccelerator::CheckDecoderDxvaSupport() {
  base::win::ScopedComPtr<IMFAttributes> attributes;
  HRESULT hr = decoder_->GetAttributes(attributes.Receive());
  RETURN_ON_HR_FAILURE(hr, "Failed to get decoder attributes", false);

  UINT32 dxva = 0;
  hr = attributes->GetUINT32(MF_SA_D3D_AWARE, &dxva);
  RETURN_ON_HR_FAILURE(hr, "Failed to check if decoder supports DXVA", false);

  hr = attributes->SetUINT32(CODECAPI_AVDecVideoAcceleration_H264, TRUE);
  RETURN_ON_HR_FAILURE(hr, "Failed to enable DXVA H/W decoding", false);
  return true;
}
