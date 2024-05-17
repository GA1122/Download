OSCL_EXPORT_REF Bool PVDecodeVideoFrame(VideoDecControls *decCtrl, uint8 *buffer[],
 uint32 timestamp[], int32 buffer_size[], uint use_ext_timestamp[], uint8 *currYUV)
{
    PV_STATUS status = PV_FAIL;
 VopHeaderInfo header_info;

    status = (PV_STATUS)PVDecodeVopHeader(decCtrl, buffer, timestamp, buffer_size, &header_info, use_ext_timestamp, currYUV);
 if (status != PV_TRUE)
 return PV_FALSE;

 if (PVDecodeVopBody(decCtrl, buffer_size) != PV_TRUE)
 {
 return PV_FALSE;
 }

 return PV_TRUE;
}
