    Read(media::mojom::VideoCaptureParamsDataView data,
         media::VideoCaptureParams* out) {
  if (!data.ReadRequestedFormat(&out->requested_format))
    return false;
  if (!data.ReadBufferType(&out->buffer_type))
    return false;
  if (!data.ReadResolutionChangePolicy(&out->resolution_change_policy))
    return false;
  if (!data.ReadPowerLineFrequency(&out->power_line_frequency))
    return false;
  return true;
}
