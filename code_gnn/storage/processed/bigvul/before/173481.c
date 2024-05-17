void omx_vdec::print_debug_extradata(OMX_OTHER_EXTRADATATYPE *extra)
{
 if (!m_debug_extradata || !extra)
 return;


    DEBUG_PRINT_HIGH(
 "============== Extra Data ==============\n"
 "           Size: %u\n"
 "        Version: %u\n"
 "      PortIndex: %u\n"
 "           Type: %x\n"
 "       DataSize: %u",
 (unsigned int)extra->nSize, (unsigned int)extra->nVersion.nVersion,
 (unsigned int)extra->nPortIndex, extra->eType, (unsigned int)extra->nDataSize);

 if (extra->eType == (OMX_EXTRADATATYPE)OMX_ExtraDataInterlaceFormat) {
        OMX_STREAMINTERLACEFORMAT *intfmt = (OMX_STREAMINTERLACEFORMAT *)(void *)extra->data;
        DEBUG_PRINT_HIGH(
 "------ Interlace Format ------\n"
 "                Size: %u\n"
 "             Version: %u\n"
 "           PortIndex: %u\n"
 " Is Interlace Format: %d\n"
 "   Interlace Formats: %u\n"
 "=========== End of Interlace ===========",
 (unsigned int)intfmt->nSize, (unsigned int)intfmt->nVersion.nVersion, (unsigned int)intfmt->nPortIndex,
                intfmt->bInterlaceFormat, (unsigned int)intfmt->nInterlaceFormats);
 } else if (extra->eType == (OMX_EXTRADATATYPE)OMX_ExtraDataFrameInfo) {
        OMX_QCOM_EXTRADATA_FRAMEINFO *fminfo = (OMX_QCOM_EXTRADATA_FRAMEINFO *)(void *)extra->data;

        DEBUG_PRINT_HIGH(
 "-------- Frame Format --------\n"
 "             Picture Type: %d\n"
 "           Interlace Type: %d\n"
 " Pan Scan Total Frame Num: %u\n"
 "   Concealed Macro Blocks: %u\n"
 "               frame rate: %u\n"
 "               Time Stamp: %llu\n"
 "           Aspect Ratio X: %u\n"
 "           Aspect Ratio Y: %u",
                fminfo->ePicType,
                fminfo->interlaceType,
 (unsigned int)fminfo->panScan.numWindows,
 (unsigned int)fminfo->nConcealedMacroblocks,
 (unsigned int)fminfo->nFrameRate,
                fminfo->nTimeStamp,
 (unsigned int)fminfo->aspectRatio.aspectRatioX,
 (unsigned int)fminfo->aspectRatio.aspectRatioY);

 for (OMX_U32 i = 0; i < fminfo->panScan.numWindows; i++) {
            DEBUG_PRINT_HIGH(
 "------------------------------"
 "     Pan Scan Frame Num: %u\n"
 "            Rectangle x: %d\n"
 "            Rectangle y: %d\n"
 "           Rectangle dx: %d\n"
 "           Rectangle dy: %d",
 (unsigned int)i, (unsigned int)fminfo->panScan.window[i].x, (unsigned int)fminfo->panScan.window[i].y,
 (unsigned int)fminfo->panScan.window[i].dx, (unsigned int)fminfo->panScan.window[i].dy);
 }

        DEBUG_PRINT_HIGH("========= End of Frame Format ==========");
 } else if (extra->eType == (OMX_EXTRADATATYPE)OMX_ExtraDataFramePackingArrangement) {
        OMX_QCOM_FRAME_PACK_ARRANGEMENT *framepack = (OMX_QCOM_FRAME_PACK_ARRANGEMENT *)(void *)extra->data;
        DEBUG_PRINT_HIGH(
 "------------------ Framepack Format ----------\n"
 "                           id: %u \n"
 "                  cancel_flag: %u \n"
 "                         type: %u \n"
 " quincunx_sampling_flagFormat: %u \n"
 "  content_interpretation_type: %u \n"
 "        spatial_flipping_flag: %u \n"
 "          frame0_flipped_flag: %u \n"
 "             field_views_flag: %u \n"
 " current_frame_is_frame0_flag: %u \n"
 "   frame0_self_contained_flag: %u \n"
 "   frame1_self_contained_flag: %u \n"
 "       frame0_grid_position_x: %u \n"
 "       frame0_grid_position_y: %u \n"
 "       frame1_grid_position_x: %u \n"
 "       frame1_grid_position_y: %u \n"
 "                reserved_byte: %u \n"
 "            repetition_period: %u \n"
 "               extension_flag: %u \n"
 "================== End of Framepack ===========",
 (unsigned int)framepack->id,
 (unsigned int)framepack->cancel_flag,
 (unsigned int)framepack->type,
 (unsigned int)framepack->quincunx_sampling_flag,
 (unsigned int)framepack->content_interpretation_type,
 (unsigned int)framepack->spatial_flipping_flag,
 (unsigned int)framepack->frame0_flipped_flag,
 (unsigned int)framepack->field_views_flag,
 (unsigned int)framepack->current_frame_is_frame0_flag,
 (unsigned int)framepack->frame0_self_contained_flag,
 (unsigned int)framepack->frame1_self_contained_flag,
 (unsigned int)framepack->frame0_grid_position_x,
 (unsigned int)framepack->frame0_grid_position_y,
 (unsigned int)framepack->frame1_grid_position_x,
 (unsigned int)framepack->frame1_grid_position_y,
 (unsigned int)framepack->reserved_byte,
 (unsigned int)framepack->repetition_period,
 (unsigned int)framepack->extension_flag);
 } else if (extra->eType == (OMX_EXTRADATATYPE)OMX_ExtraDataQP) {
        OMX_QCOM_EXTRADATA_QP * qp = (OMX_QCOM_EXTRADATA_QP *)(void *)extra->data;
        DEBUG_PRINT_HIGH(
 "---- QP (Frame quantization parameter) ----\n"
 "    Frame QP: %u \n"
 "================ End of QP ================\n",
 (unsigned int)qp->nQP);
 } else if (extra->eType == (OMX_EXTRADATATYPE)OMX_ExtraDataInputBitsInfo) {
        OMX_QCOM_EXTRADATA_BITS_INFO * bits = (OMX_QCOM_EXTRADATA_BITS_INFO *)(void *)extra->data;
        DEBUG_PRINT_HIGH(
 "--------- Input bits information --------\n"
 "    Header bits: %u \n"
 "     Frame bits: %u \n"
 "===== End of Input bits information =====\n",
 (unsigned int)bits->header_bits, (unsigned int)bits->frame_bits);
 } else if (extra->eType == (OMX_EXTRADATATYPE)OMX_ExtraDataMP2UserData) {
        OMX_QCOM_EXTRADATA_USERDATA *userdata = (OMX_QCOM_EXTRADATA_USERDATA *)(void *)extra->data;
        OMX_U8 *data_ptr = (OMX_U8 *)userdata->data;
        OMX_U32 userdata_size = extra->nDataSize - sizeof(userdata->type);
        OMX_U32 i = 0;
        DEBUG_PRINT_HIGH(
 "--------------  Userdata  -------------\n"
 "    Stream userdata type: %u\n"
 "          userdata size: %u\n"
 "    STREAM_USERDATA:",
 (unsigned int)userdata->type, (unsigned int)userdata_size);
 for (i = 0; i < userdata_size; i+=4) {
                    DEBUG_PRINT_HIGH("        %x %x %x %x",
                        data_ptr[i], data_ptr[i+1],
                        data_ptr[i+2], data_ptr[i+3]);
 }
        DEBUG_PRINT_HIGH(
 "=========== End of Userdata ===========");
 } else if (extra->eType == (OMX_EXTRADATATYPE)OMX_ExtraDataMpeg2SeqDisplay) {
        OMX_QCOM_EXTRADATA_MPEG2SEQDISPLAY *seq_display = (OMX_QCOM_EXTRADATA_MPEG2SEQDISPLAY*)(void*)extra->data;
        DEBUG_PRINT_HIGH(
 "------Mpeg2SeqDisplay ------\n"
 "     Frame Width: %d\n"
 "    Frame Height: %d\n"
 "=========== End of Mpeg2SeqDisplay ===========",
                seq_display->disp_width, seq_display->disp_height);
 } else if (extra->eType == OMX_ExtraDataNone) {
        DEBUG_PRINT_HIGH("========== End of Terminator ===========");
 } else {
        DEBUG_PRINT_HIGH("======= End of Driver Extradata ========");
 }
}
