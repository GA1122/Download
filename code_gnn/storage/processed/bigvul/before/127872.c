MediaContainerName DetermineContainer(const uint8* buffer, int buffer_size) {
  DCHECK(buffer);

  if (CheckMov(buffer, buffer_size))
    return CONTAINER_MOV;

  MediaContainerName result = LookupContainerByFirst4(buffer, buffer_size);
  if (result != CONTAINER_UNKNOWN)
    return result;

  if (CheckMpeg2ProgramStream(buffer, buffer_size))
    return CONTAINER_MPEG2PS;
  if (CheckMpeg2TransportStream(buffer, buffer_size))
    return CONTAINER_MPEG2TS;
  if (CheckMJpeg(buffer, buffer_size))
    return CONTAINER_MJPEG;
  if (CheckDV(buffer, buffer_size))
    return CONTAINER_DV;
  if (CheckH261(buffer, buffer_size))
    return CONTAINER_H261;
  if (CheckH263(buffer, buffer_size))
    return CONTAINER_H263;
  if (CheckH264(buffer, buffer_size))
    return CONTAINER_H264;
  if (CheckMpeg4BitStream(buffer, buffer_size))
    return CONTAINER_MPEG4BS;
  if (CheckVC1(buffer, buffer_size))
    return CONTAINER_VC1;
  if (CheckSrt(buffer, buffer_size))
    return CONTAINER_SRT;
  if (CheckGsm(buffer, buffer_size))
    return CONTAINER_GSM;

  int offset = 1;   
  if (AdvanceToStartCode(buffer, buffer_size, &offset, 4, 16, kAc3SyncWord)) {
    if (CheckAc3(buffer + offset, buffer_size - offset))
      return CONTAINER_AC3;
    if (CheckEac3(buffer + offset, buffer_size - offset))
      return CONTAINER_EAC3;
  }

  return CONTAINER_UNKNOWN;
}
