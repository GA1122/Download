bool DXVAVideoDecodeAccelerator::SetDecoderInputMediaType() {
  base::win::ScopedComPtr<IMFMediaType> media_type;
  HRESULT hr = MFCreateMediaType(media_type.Receive());
  RETURN_ON_HR_FAILURE(hr, "MFCreateMediaType failed", false);

  hr = media_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
  RETURN_ON_HR_FAILURE(hr, "Failed to set major input type", false);

  hr = media_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
  RETURN_ON_HR_FAILURE(hr, "Failed to set subtype", false);

  hr = decoder_->SetInputType(0, media_type, 0);   
  RETURN_ON_HR_FAILURE(hr, "Failed to set decoder input type", false);
  return true;
}
