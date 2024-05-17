void DXVAVideoDecodeAccelerator::DoDecode() {
  RETURN_AND_NOTIFY_ON_FAILURE((state_ == kNormal || state_ == kEosDrain),
      "DoDecode: not in normal/drain state", ILLEGAL_STATE,);

  MFT_OUTPUT_DATA_BUFFER output_data_buffer = {0};
  DWORD status = 0;

  HRESULT hr = decoder_->ProcessOutput(0,   
                                       1,   
                                       &output_data_buffer,
                                       &status);
  IMFCollection* events = output_data_buffer.pEvents;
  if (events != NULL) {
    VLOG(1) << "Got events from ProcessOuput, but discarding";
    events->Release();
  }
  if (FAILED(hr)) {
    if (hr == MF_E_TRANSFORM_STREAM_CHANGE) {
      if (!SetDecoderOutputMediaType(MFVideoFormat_NV12)) {
        NOTREACHED() << "Failed to set decoder output media type to NV12";
        state_ = kStopped;
      } else {
        DVLOG(1) << "Received output format change from the decoder."
                    " Recursively invoking DoDecode";
        DoDecode();
      }
      return;
    } else if (hr == MF_E_TRANSFORM_NEED_MORE_INPUT) {
      state_ = kStopped;
      return;
    } else {
      NOTREACHED() << "Unhandled error in DoDecode()";
      return;
    }
  }
  TRACE_EVENT_END_ETW("DXVAVideoDecodeAccelerator.Decoding", this, "");

  TRACE_COUNTER1("DXVA Decoding", "TotalPacketsBeforeDecode",
                 inputs_before_decode_);

  inputs_before_decode_ = 0;

  RETURN_AND_NOTIFY_ON_FAILURE(ProcessOutputSample(output_data_buffer.pSample),
      "Failed to process output sample.", PLATFORM_FAILURE,);

  state_ = kNormal;
}
