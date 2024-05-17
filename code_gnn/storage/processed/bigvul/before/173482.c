void omx_vdec::process_event_cb(void *ctxt, unsigned char id)
{
 unsigned long p1;  
 unsigned long p2;  
 unsigned long ident;
 unsigned qsize=0;  
    omx_vdec *pThis = (omx_vdec *) ctxt;

 if (!pThis) {
        DEBUG_PRINT_ERROR("ERROR: %s()::Context is incorrect, bailing out",
                __func__);
 return;
 }

 do {
  
        pthread_mutex_lock(&pThis->m_lock);
        qsize = pThis->m_cmd_q.m_size;
 if (qsize) {
            pThis->m_cmd_q.pop_entry(&p1, &p2, &ident);
 }

 if (qsize == 0 && pThis->m_state != OMX_StatePause) {
            qsize = pThis->m_ftb_q.m_size;
 if (qsize) {
                pThis->m_ftb_q.pop_entry(&p1, &p2, &ident);
 }
 }

 if (qsize == 0 && pThis->m_state != OMX_StatePause) {
            qsize = pThis->m_etb_q.m_size;
 if (qsize) {
                pThis->m_etb_q.pop_entry(&p1, &p2, &ident);
 }
 }
        pthread_mutex_unlock(&pThis->m_lock);

  
 if (qsize > 0) {
            id = ident;
 switch (id) {
 case OMX_COMPONENT_GENERATE_EVENT:
 if (pThis->m_cb.EventHandler) {
 switch (p1) {
 case OMX_CommandStateSet:
                                pThis->m_state = (OMX_STATETYPE) p2;
                                DEBUG_PRINT_HIGH("OMX_CommandStateSet complete, m_state = %d",
                                        pThis->m_state);
                                pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                        OMX_EventCmdComplete, p1, p2, NULL);
 break;

 case OMX_EventError:
 if (p2 == OMX_StateInvalid) {
                                    DEBUG_PRINT_ERROR("OMX_EventError: p2 is OMX_StateInvalid");
                                    pThis->m_state = (OMX_STATETYPE) p2;
                                    pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                            OMX_EventError, OMX_ErrorInvalidState, p2, NULL);
 } else if (p2 == (unsigned long)OMX_ErrorHardware) {
                                    pThis->omx_report_error();
 } else {
                                    pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                            OMX_EventError, p2, (OMX_U32)NULL, NULL );
 }
 break;

 case OMX_CommandPortDisable:
                                DEBUG_PRINT_HIGH("OMX_CommandPortDisable complete for port [%lu]", p2);
 if (BITMASK_PRESENT(&pThis->m_flags,
                                            OMX_COMPONENT_OUTPUT_FLUSH_IN_DISABLE_PENDING)) {
                                    BITMASK_SET(&pThis->m_flags, OMX_COMPONENT_DISABLE_OUTPUT_DEFERRED);
 break;
 }
 if (p2 == OMX_CORE_OUTPUT_PORT_INDEX) {
                                    OMX_ERRORTYPE eRet = OMX_ErrorNone;
                                    pThis->stream_off(OMX_CORE_OUTPUT_PORT_INDEX);
 if (release_buffers(pThis, VDEC_BUFFER_TYPE_OUTPUT))
                                        DEBUG_PRINT_HIGH("Failed to release output buffers");
                                    OMX_ERRORTYPE eRet1 = pThis->get_buffer_req(&pThis->drv_ctx.op_buf);
                                    pThis->in_reconfig = false;
 if (eRet !=  OMX_ErrorNone) {
                                        DEBUG_PRINT_ERROR("set_buffer_req failed eRet = %d",eRet);
                                        pThis->omx_report_error();
 break;
 }
 }
                                pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                        OMX_EventCmdComplete, p1, p2, NULL );
 break;
 case OMX_CommandPortEnable:
                                DEBUG_PRINT_HIGH("OMX_CommandPortEnable complete for port [%lu]", p2);
                                pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,\
                                        OMX_EventCmdComplete, p1, p2, NULL );
 break;

 default:
                                pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                        OMX_EventCmdComplete, p1, p2, NULL );
 break;

 }
 } else {
                        DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }
 break;
 case OMX_COMPONENT_GENERATE_ETB_ARBITRARY:
 if (pThis->empty_this_buffer_proxy_arbitrary((OMX_HANDLETYPE)p1,\
 (OMX_BUFFERHEADERTYPE *)(intptr_t)p2) != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("empty_this_buffer_proxy_arbitrary failure");
                        pThis->omx_report_error ();
 }
 break;
 case OMX_COMPONENT_GENERATE_ETB: {
                        OMX_ERRORTYPE iret;
                        iret = pThis->empty_this_buffer_proxy((OMX_HANDLETYPE)p1, (OMX_BUFFERHEADERTYPE *)p2);
 if (iret == OMX_ErrorInsufficientResources) {
                            DEBUG_PRINT_ERROR("empty_this_buffer_proxy failure due to HW overload");
                            pThis->omx_report_hw_overload ();
 } else if (iret != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("empty_this_buffer_proxy failure");
                        pThis->omx_report_error ();
 }
 }
 break;

 case OMX_COMPONENT_GENERATE_FTB:
 if ( pThis->fill_this_buffer_proxy((OMX_HANDLETYPE)(intptr_t)p1,\
 (OMX_BUFFERHEADERTYPE *)(intptr_t)p2) != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("fill_this_buffer_proxy failure");
                        pThis->omx_report_error ();
 }
 break;

 case OMX_COMPONENT_GENERATE_COMMAND:
                    pThis->send_command_proxy(&pThis->m_cmp,(OMX_COMMANDTYPE)p1,\
 (OMX_U32)p2,(OMX_PTR)NULL);
 break;

 case OMX_COMPONENT_GENERATE_EBD:

 if (p2 != VDEC_S_SUCCESS && p2 != VDEC_S_INPUT_BITSTREAM_ERR) {
                        DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_EBD failure");
                        pThis->omx_report_error ();
 } else {
 if (p2 == VDEC_S_INPUT_BITSTREAM_ERR && p1) {
                            pThis->time_stamp_dts.remove_time_stamp(
 ((OMX_BUFFERHEADERTYPE *)(intptr_t)p1)->nTimeStamp,
 (pThis->drv_ctx.interlace != VDEC_InterlaceFrameProgressive)
 ?true:false);
 }
 if ( pThis->empty_buffer_done(&pThis->m_cmp,
 (OMX_BUFFERHEADERTYPE *)(intptr_t)p1) != OMX_ErrorNone) {
                            DEBUG_PRINT_ERROR("empty_buffer_done failure");
                            pThis->omx_report_error ();
 }
 }
 break;
 case OMX_COMPONENT_GENERATE_INFO_FIELD_DROPPED: {
 int64_t *timestamp = (int64_t *)(intptr_t)p1;
 if (p1) {
                                                pThis->time_stamp_dts.remove_time_stamp(*timestamp,
 (pThis->drv_ctx.interlace != VDEC_InterlaceFrameProgressive)
 ?true:false);
                                                free(timestamp);
 }
 }
 break;
 case OMX_COMPONENT_GENERATE_FBD:
 if (p2 != VDEC_S_SUCCESS) {
                                            DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_FBD failure");
                                            pThis->omx_report_error ();
 } else if ( pThis->fill_buffer_done(&pThis->m_cmp,
 (OMX_BUFFERHEADERTYPE *)(intptr_t)p1) != OMX_ErrorNone ) {
                                            DEBUG_PRINT_ERROR("fill_buffer_done failure");
                                            pThis->omx_report_error ();
 }
 break;

 case OMX_COMPONENT_GENERATE_EVENT_INPUT_FLUSH:
                                        DEBUG_PRINT_HIGH("Driver flush i/p Port complete");
 if (!pThis->input_flush_progress) {
                                            DEBUG_PRINT_HIGH("WARNING: Unexpected flush from driver");
 } else {
                                            pThis->execute_input_flush();
 if (pThis->m_cb.EventHandler) {
 if (p2 != VDEC_S_SUCCESS) {
                                                    DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_EVENT_INPUT_FLUSH failure");
                                                    pThis->omx_report_error ();
 } else {
  
 if (BITMASK_PRESENT(&pThis->m_flags,
                                                                OMX_COMPONENT_INPUT_FLUSH_PENDING)) {
                                                        BITMASK_CLEAR (&pThis->m_flags,OMX_COMPONENT_INPUT_FLUSH_PENDING);
                                                        DEBUG_PRINT_LOW("Input Flush completed - Notify Client");
                                                        pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                                                OMX_EventCmdComplete,OMX_CommandFlush,
                                                                OMX_CORE_INPUT_PORT_INDEX,NULL );
 }
 if (BITMASK_PRESENT(&pThis->m_flags,
                                                                OMX_COMPONENT_IDLE_PENDING)) {
 if (pThis->stream_off(OMX_CORE_INPUT_PORT_INDEX)) {
                                                            DEBUG_PRINT_ERROR("Failed to call streamoff on OUTPUT Port");
                                                            pThis->omx_report_error ();
 } else {
                                                            pThis->streaming[OUTPUT_PORT] = false;
 }
 if (!pThis->output_flush_progress) {
                                                            DEBUG_PRINT_LOW("Input flush done hence issue stop");
                                                            pThis->post_event ((unsigned int)NULL, VDEC_S_SUCCESS,\
                                                                    OMX_COMPONENT_GENERATE_STOP_DONE);
 }
 }
 }
 } else {
                                                DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }
 }
 break;

 case OMX_COMPONENT_GENERATE_EVENT_OUTPUT_FLUSH:
                                        DEBUG_PRINT_HIGH("Driver flush o/p Port complete");
 if (!pThis->output_flush_progress) {
                                            DEBUG_PRINT_HIGH("WARNING: Unexpected flush from driver");
 } else {
                                            pThis->execute_output_flush();
 if (pThis->m_cb.EventHandler) {
 if (p2 != VDEC_S_SUCCESS) {
                                                    DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_EVENT_OUTPUT_FLUSH failed");
                                                    pThis->omx_report_error ();
 } else {
  
 if (BITMASK_PRESENT(&pThis->m_flags,
                                                                OMX_COMPONENT_OUTPUT_FLUSH_PENDING)) {
                                                        DEBUG_PRINT_LOW("Notify Output Flush done");
                                                        BITMASK_CLEAR (&pThis->m_flags,OMX_COMPONENT_OUTPUT_FLUSH_PENDING);
                                                        pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                                                OMX_EventCmdComplete,OMX_CommandFlush,
                                                                OMX_CORE_OUTPUT_PORT_INDEX,NULL );
 }
 if (BITMASK_PRESENT(&pThis->m_flags,
                                                                OMX_COMPONENT_OUTPUT_FLUSH_IN_DISABLE_PENDING)) {
                                                        DEBUG_PRINT_LOW("Internal flush complete");
                                                        BITMASK_CLEAR (&pThis->m_flags,
                                                                OMX_COMPONENT_OUTPUT_FLUSH_IN_DISABLE_PENDING);
 if (BITMASK_PRESENT(&pThis->m_flags,
                                                                    OMX_COMPONENT_DISABLE_OUTPUT_DEFERRED)) {
                                                            pThis->post_event(OMX_CommandPortDisable,
                                                                    OMX_CORE_OUTPUT_PORT_INDEX,
                                                                    OMX_COMPONENT_GENERATE_EVENT);
                                                            BITMASK_CLEAR (&pThis->m_flags,
                                                                    OMX_COMPONENT_DISABLE_OUTPUT_DEFERRED);
                                                            BITMASK_CLEAR (&pThis->m_flags,
                                                                    OMX_COMPONENT_OUTPUT_DISABLE_PENDING);

 }
 }

 if (BITMASK_PRESENT(&pThis->m_flags ,OMX_COMPONENT_IDLE_PENDING)) {
 if (pThis->stream_off(OMX_CORE_OUTPUT_PORT_INDEX)) {
                                                            DEBUG_PRINT_ERROR("Failed to call streamoff on CAPTURE Port");
                                                            pThis->omx_report_error ();
 break;
 }
                                                        pThis->streaming[CAPTURE_PORT] = false;
 if (!pThis->input_flush_progress) {
                                                            DEBUG_PRINT_LOW("Output flush done hence issue stop");
                                                            pThis->post_event ((unsigned int)NULL, VDEC_S_SUCCESS,\
                                                                    OMX_COMPONENT_GENERATE_STOP_DONE);
 }
 }
 }
 } else {
                                                DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }
 }
 break;

 case OMX_COMPONENT_GENERATE_START_DONE:
                                        DEBUG_PRINT_HIGH("Rxd OMX_COMPONENT_GENERATE_START_DONE");

 if (pThis->m_cb.EventHandler) {
 if (p2 != VDEC_S_SUCCESS) {
                                                DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_START_DONE Failure");
                                                pThis->omx_report_error ();
 } else {
                                                DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_START_DONE Success");
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_EXECUTE_PENDING)) {
                                                    DEBUG_PRINT_LOW("Move to executing");
                                                    BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_EXECUTE_PENDING);
                                                    pThis->m_state = OMX_StateExecuting;
                                                    pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                                            OMX_EventCmdComplete,OMX_CommandStateSet,
                                                            OMX_StateExecuting, NULL);
 } else if (BITMASK_PRESENT(&pThis->m_flags,
                                                            OMX_COMPONENT_PAUSE_PENDING)) {
 if ( 0) {
                                                        DEBUG_PRINT_ERROR("VDEC_IOCTL_CMD_PAUSE failed");
                                                        pThis->omx_report_error ();
 }
 }
 }
 } else {
                                            DEBUG_PRINT_LOW("Event Handler callback is NULL");
 }
 break;

 case OMX_COMPONENT_GENERATE_PAUSE_DONE:
                                        DEBUG_PRINT_HIGH("Rxd OMX_COMPONENT_GENERATE_PAUSE_DONE");
 if (pThis->m_cb.EventHandler) {
 if (p2 != VDEC_S_SUCCESS) {
                                                DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_PAUSE_DONE ret failed");
                                                pThis->omx_report_error ();
 } else {
                                                pThis->complete_pending_buffer_done_cbs();
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_PAUSE_PENDING)) {
                                                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_PAUSE_DONE nofity");
                                                    BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_PAUSE_PENDING);
                                                    pThis->m_state = OMX_StatePause;
                                                    pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                                            OMX_EventCmdComplete,OMX_CommandStateSet,
                                                            OMX_StatePause, NULL);
 }
 }
 } else {
                                            DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }

 break;

 case OMX_COMPONENT_GENERATE_RESUME_DONE:
                                        DEBUG_PRINT_HIGH("Rxd OMX_COMPONENT_GENERATE_RESUME_DONE");
 if (pThis->m_cb.EventHandler) {
 if (p2 != VDEC_S_SUCCESS) {
                                                DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_RESUME_DONE failed");
                                                pThis->omx_report_error ();
 } else {
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_EXECUTE_PENDING)) {
                                                    DEBUG_PRINT_LOW("Moving the decoder to execute state");
                                                    BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_EXECUTE_PENDING);
                                                    pThis->m_state = OMX_StateExecuting;
                                                    pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                                            OMX_EventCmdComplete,OMX_CommandStateSet,
                                                            OMX_StateExecuting,NULL);
 }
 }
 } else {
                                            DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }

 break;

 case OMX_COMPONENT_GENERATE_STOP_DONE:
                                        DEBUG_PRINT_HIGH("Rxd OMX_COMPONENT_GENERATE_STOP_DONE");
 if (pThis->m_cb.EventHandler) {
 if (p2 != VDEC_S_SUCCESS) {
                                                DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_STOP_DONE ret failed");
                                                pThis->omx_report_error ();
 } else {
                                                pThis->complete_pending_buffer_done_cbs();
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_IDLE_PENDING)) {
                                                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_STOP_DONE Success");
                                                    BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_IDLE_PENDING);
                                                    pThis->m_state = OMX_StateIdle;
                                                    DEBUG_PRINT_LOW("Move to Idle State");
                                                    pThis->m_cb.EventHandler(&pThis->m_cmp,pThis->m_app_data,
                                                            OMX_EventCmdComplete,OMX_CommandStateSet,
                                                            OMX_StateIdle,NULL);
 }
 }
 } else {
                                            DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }

 break;

 case OMX_COMPONENT_GENERATE_PORT_RECONFIG:
 if (p2 == OMX_IndexParamPortDefinition) {
                                            DEBUG_PRINT_HIGH("Rxd PORT_RECONFIG: OMX_IndexParamPortDefinition");
                                            pThis->in_reconfig = true;

 } else if (p2 == OMX_IndexConfigCommonOutputCrop) {
                                            DEBUG_PRINT_HIGH("Rxd PORT_RECONFIG: OMX_IndexConfigCommonOutputCrop");

  
 if (pThis->m_smoothstreaming_mode &&
 (pThis->framesize.nWidth !=
                                                    pThis->drv_ctx.video_resolution.frame_width) ||
 (pThis->framesize.nHeight !=
                                                    pThis->drv_ctx.video_resolution.frame_height)) {

                                                DEBUG_PRINT_HIGH("Resolution changed from: wxh = %dx%d to: wxh = %dx%d",
                                                        pThis->framesize.nWidth,
                                                        pThis->framesize.nHeight,
                                                        pThis->drv_ctx.video_resolution.frame_width,
                                                        pThis->drv_ctx.video_resolution.frame_height);

  
                                                pThis->framesize.nWidth =
                                                       pThis->drv_ctx.video_resolution.frame_width;
                                                pThis->framesize.nHeight =
                                                       pThis->drv_ctx.video_resolution.frame_height;

  
 if (!pThis->client_buffers.update_buffer_req()) {
                                                    DEBUG_PRINT_ERROR("Setting C2D buffer requirements failed");
 }
 }

  
                                            pThis->rectangle.nLeft = pThis->drv_ctx.frame_size.left;
                                            pThis->rectangle.nTop = pThis->drv_ctx.frame_size.top;
                                            pThis->rectangle.nWidth = pThis->drv_ctx.frame_size.right;
                                            pThis->rectangle.nHeight = pThis->drv_ctx.frame_size.bottom;

  
 if (pThis->rectangle.nLeft + pThis->rectangle.nWidth >
                                                pThis->drv_ctx.video_resolution.frame_width) {

                                                DEBUG_PRINT_HIGH("Crop L[%u] + R[%u] > W[%u]",
                                                        pThis->rectangle.nLeft, pThis->rectangle.nWidth,
                                                        pThis->drv_ctx.video_resolution.frame_width);
                                                pThis->rectangle.nLeft = 0;

 if (pThis->rectangle.nWidth >
                                                    pThis->drv_ctx.video_resolution.frame_width) {

                                                    DEBUG_PRINT_HIGH("Crop R[%u] > W[%u]",
                                                            pThis->rectangle.nWidth,
                                                            pThis->drv_ctx.video_resolution.frame_width);
                                                    pThis->rectangle.nWidth =
                                                        pThis->drv_ctx.video_resolution.frame_width;
 }
 }
 if (pThis->rectangle.nTop + pThis->rectangle.nHeight >
                                                pThis->drv_ctx.video_resolution.frame_height) {

                                                DEBUG_PRINT_HIGH("Crop T[%u] + B[%u] > H[%u]",
                                                    pThis->rectangle.nTop, pThis->rectangle.nHeight,
                                                    pThis->drv_ctx.video_resolution.frame_height);
                                                pThis->rectangle.nTop = 0;

 if (pThis->rectangle.nHeight >
                                                    pThis->drv_ctx.video_resolution.frame_height) {

                                                    DEBUG_PRINT_HIGH("Crop B[%u] > H[%u]",
                                                        pThis->rectangle.nHeight,
                                                        pThis->drv_ctx.video_resolution.frame_height);
                                                    pThis->rectangle.nHeight =
                                                        pThis->drv_ctx.video_resolution.frame_height;
 }
 }
                                            DEBUG_PRINT_HIGH("Updated Crop Info: L: %u, T: %u, R: %u, B: %u",
                                                    pThis->rectangle.nLeft, pThis->rectangle.nTop,
                                                    pThis->rectangle.nWidth, pThis->rectangle.nHeight);
 } else {
                                            DEBUG_PRINT_ERROR("Rxd Invalid PORT_RECONFIG event (%lu)", p2);
 break;
 }
 if (pThis->m_cb.EventHandler) {
                                            pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                                    OMX_EventPortSettingsChanged, p1, p2, NULL );
 } else {
                                            DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }

 break;

 case OMX_COMPONENT_GENERATE_EOS_DONE:
                                        DEBUG_PRINT_HIGH("Rxd OMX_COMPONENT_GENERATE_EOS_DONE");
 if (pThis->m_cb.EventHandler) {
                                            pThis->m_cb.EventHandler(&pThis->m_cmp, pThis->m_app_data, OMX_EventBufferFlag,
                                                    OMX_CORE_OUTPUT_PORT_INDEX, OMX_BUFFERFLAG_EOS, NULL );
 } else {
                                            DEBUG_PRINT_ERROR("ERROR: %s()::EventHandler is NULL", __func__);
 }
                                        pThis->prev_ts = LLONG_MAX;
                                        pThis->rst_prev_ts = true;
 break;

 case OMX_COMPONENT_GENERATE_HARDWARE_ERROR:
                                        DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_HARDWARE_ERROR");
                                        pThis->omx_report_error ();
 break;

 case OMX_COMPONENT_GENERATE_UNSUPPORTED_SETTING:
                                        DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_UNSUPPORTED_SETTING");
                                        pThis->omx_report_unsupported_setting();
 break;

 case OMX_COMPONENT_GENERATE_HARDWARE_OVERLOAD:
                                        DEBUG_PRINT_ERROR("OMX_COMPONENT_GENERATE_HARDWARE_OVERLOAD");
                                        pThis->omx_report_hw_overload();
 break;

 default:
 break;
 }
 }
        pthread_mutex_lock(&pThis->m_lock);
        qsize = pThis->m_cmd_q.m_size;
 if (pThis->m_state != OMX_StatePause)
            qsize += (pThis->m_ftb_q.m_size + pThis->m_etb_q.m_size);
        pthread_mutex_unlock(&pThis->m_lock);
 } while (qsize>0);

}
