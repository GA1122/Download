bool DXVAVideoDecodeAccelerator::SendMFTMessage(MFT_MESSAGE_TYPE msg,
                                                int32 param) {
  HRESULT hr = decoder_->ProcessMessage(msg, param);
  return SUCCEEDED(hr);
}
