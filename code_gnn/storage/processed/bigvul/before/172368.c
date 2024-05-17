void omx_video::process_event_cb(void *ctxt, unsigned char id)
{
 unsigned long p1;  
 unsigned long p2;  
 unsigned long ident;
 unsigned qsize=0;  
    omx_video *pThis = (omx_video *) ctxt;

 if (!pThis) {
        DEBUG_PRINT_ERROR("ERROR:ProcessMsgCb:Context is incorrect; bailing out");
 return;
 }

 do {
  

        pthread_mutex_lock(&pThis->m_lock);
        qsize = pThis->m_cmd_q.m_size;
 if (qsize) {
            pThis->m_cmd_q.pop_entry(&p1,&p2,&ident);
 }

 if (qsize == 0) {
            qsize = pThis->m_ftb_q.m_size;
 if (qsize) {
                pThis->m_ftb_q.pop_entry(&p1,&p2,&ident);
 }
 }

 if (qsize == 0) {
            qsize = pThis->m_etb_q.m_size;
 if (qsize) {
                pThis->m_etb_q.pop_entry(&p1,&p2,&ident);
 }
 }

        pthread_mutex_unlock(&pThis->m_lock);

  
 if (qsize > 0) {
            id = ident;
 switch (id) {
 case OMX_COMPONENT_GENERATE_EVENT:
 if (pThis->m_pCallbacks.EventHandler) {
 switch (p1) {
 case OMX_CommandStateSet:
                                pThis->m_state = (OMX_STATETYPE) p2;
                                DEBUG_PRINT_LOW("Process -> state set to %d", pThis->m_state);
                                pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                        OMX_EventCmdComplete, p1, p2, NULL);
 break;

 case OMX_EventError:
                                DEBUG_PRINT_ERROR("ERROR: OMX_EventError: p2 = %lu", p2);
 if (p2 == (unsigned)OMX_ErrorHardware) {
                                    pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                            OMX_EventError,OMX_ErrorHardware,0,NULL);
 } else {
                                    pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                            OMX_EventError, p2, 0, 0);

 }
 break;

 case OMX_CommandPortDisable:
                                DEBUG_PRINT_LOW("Process -> Port %lu set to PORT_STATE_DISABLED" \
 "state", p2);
                                pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                        OMX_EventCmdComplete, p1, p2, NULL );
 break;
 case OMX_CommandPortEnable:
                                DEBUG_PRINT_LOW("Process ->Port %lu set PORT_STATE_ENABLED state" \
 , p2);
                                pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,\
                                        OMX_EventCmdComplete, p1, p2, NULL );
 break;

 default:
                                DEBUG_PRINT_LOW("process_event_cb forwarding EventCmdComplete %lu", p1);
                                pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                        OMX_EventCmdComplete, p1, p2, NULL );
 break;

 }
 } else {
                        DEBUG_PRINT_ERROR("ERROR: ProcessMsgCb NULL callbacks");
 }
 break;
 case OMX_COMPONENT_GENERATE_ETB_OPQ:
                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_ETB_OPQ");
 if (pThis->empty_this_buffer_opaque((OMX_HANDLETYPE)p1,\
 (OMX_BUFFERHEADERTYPE *)p2) != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("ERROR: ETBProxy() failed!");
                        pThis->omx_report_error ();
 }
 break;
 case OMX_COMPONENT_GENERATE_ETB: {
                        OMX_ERRORTYPE iret;
                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_ETB");
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
 if ( pThis->fill_this_buffer_proxy((OMX_HANDLETYPE)p1,\
 (OMX_BUFFERHEADERTYPE *)p2) != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("ERROR: FTBProxy() failed!");
                        pThis->omx_report_error ();
 }
 break;

 case OMX_COMPONENT_GENERATE_COMMAND:
                    pThis->send_command_proxy(&pThis->m_cmp,(OMX_COMMANDTYPE)p1,\
 (OMX_U32)p2,(OMX_PTR)NULL);
 break;

 case OMX_COMPONENT_GENERATE_EBD:
 if ( pThis->empty_buffer_done(&pThis->m_cmp,
 (OMX_BUFFERHEADERTYPE *)p1) != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("ERROR: empty_buffer_done() failed!");
                        pThis->omx_report_error ();
 }
 break;

 case OMX_COMPONENT_GENERATE_FBD:
 if ( pThis->fill_buffer_done(&pThis->m_cmp,
 (OMX_BUFFERHEADERTYPE *)p1) != OMX_ErrorNone ) {
                        DEBUG_PRINT_ERROR("ERROR: fill_buffer_done() failed!");
                        pThis->omx_report_error ();
 }
 break;

 case OMX_COMPONENT_GENERATE_EVENT_INPUT_FLUSH:

                    pThis->input_flush_progress = false;
                    DEBUG_PRINT_HIGH("m_etb_count at i/p flush = %" PRIu64, m_etb_count);
                    m_etb_count = 0;
 if (pThis->m_pCallbacks.EventHandler) {
  
 if (BITMASK_PRESENT(&pThis->m_flags,
                                    OMX_COMPONENT_INPUT_FLUSH_PENDING)) {
                            BITMASK_CLEAR (&pThis->m_flags,OMX_COMPONENT_INPUT_FLUSH_PENDING);
                            pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                    OMX_EventCmdComplete,OMX_CommandFlush,
                                    PORT_INDEX_IN,NULL );
 } else if (BITMASK_PRESENT(&pThis->m_flags,
                                    OMX_COMPONENT_IDLE_PENDING)) {
 if (!pThis->output_flush_progress) {
                                DEBUG_PRINT_LOW("dev_stop called after input flush complete");
 if (dev_stop() != 0) {
                                    DEBUG_PRINT_ERROR("ERROR: dev_stop() failed in i/p flush!");
                                    pThis->omx_report_error ();
 }
 }
 }
 }

 break;

 case OMX_COMPONENT_GENERATE_EVENT_OUTPUT_FLUSH:

                    pThis->output_flush_progress = false;
                    DEBUG_PRINT_HIGH("m_fbd_count at o/p flush = %" PRIu64, m_fbd_count);
                    m_fbd_count = 0;
 if (pThis->m_pCallbacks.EventHandler) {
  
 if (BITMASK_PRESENT(&pThis->m_flags,
                                    OMX_COMPONENT_OUTPUT_FLUSH_PENDING)) {
                            BITMASK_CLEAR (&pThis->m_flags,OMX_COMPONENT_OUTPUT_FLUSH_PENDING);

                            pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                    OMX_EventCmdComplete,OMX_CommandFlush,
                                    PORT_INDEX_OUT,NULL );
 } else if (BITMASK_PRESENT(&pThis->m_flags ,OMX_COMPONENT_IDLE_PENDING)) {
                            DEBUG_PRINT_LOW("dev_stop called after Output flush complete");
 if (!pThis->input_flush_progress) {
 if (dev_stop() != 0) {
                                    DEBUG_PRINT_ERROR("ERROR: dev_stop() failed in o/p flush!");
                                    pThis->omx_report_error ();
 }
 }
 }
 }
 break;

 case OMX_COMPONENT_GENERATE_START_DONE:
                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_START_DONE msg");

 if (pThis->m_pCallbacks.EventHandler) {
                        DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_START_DONE Success");
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_EXECUTE_PENDING)) {
                            DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_START_DONE Move to \
                                    executing");
                            BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_EXECUTE_PENDING);
                            pThis->m_state = OMX_StateExecuting;
                            pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                    OMX_EventCmdComplete,OMX_CommandStateSet,
                                    OMX_StateExecuting, NULL);
 } else if (BITMASK_PRESENT(&pThis->m_flags,
                                    OMX_COMPONENT_PAUSE_PENDING)) {
 if (dev_pause()) {
                                DEBUG_PRINT_ERROR("ERROR: dev_pause() failed in Start Done!");
                                pThis->omx_report_error ();
 }
 } else if (BITMASK_PRESENT(&pThis->m_flags,
                                    OMX_COMPONENT_LOADED_START_PENDING)) {
 if (dev_loaded_start_done()) {
                                DEBUG_PRINT_LOW("successful loaded Start Done!");
 } else {
                                DEBUG_PRINT_ERROR("ERROR: failed in loaded Start Done!");
                                pThis->omx_report_error ();
 }
                            BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_LOADED_START_PENDING);
 } else {
                            DEBUG_PRINT_LOW("ERROR: unknown flags=%" PRIx64, pThis->m_flags);
 }
 } else {
                        DEBUG_PRINT_LOW("Event Handler callback is NULL");
 }
 break;

 case OMX_COMPONENT_GENERATE_PAUSE_DONE:
                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_PAUSE_DONE msg");
 if (pThis->m_pCallbacks.EventHandler) {
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_PAUSE_PENDING)) {
                            pThis->complete_pending_buffer_done_cbs();
                            DEBUG_PRINT_LOW("omx_video::process_event_cb() Sending PAUSE complete after all pending EBD/FBD");
                            BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_PAUSE_PENDING);
                            pThis->m_state = OMX_StatePause;
                            pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                    OMX_EventCmdComplete,OMX_CommandStateSet,
                                    OMX_StatePause, NULL);
 }
 }

 break;

 case OMX_COMPONENT_GENERATE_RESUME_DONE:
                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_RESUME_DONE msg");
 if (pThis->m_pCallbacks.EventHandler) {
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_EXECUTE_PENDING)) {
                            BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_EXECUTE_PENDING);
                            pThis->m_state = OMX_StateExecuting;
                            pThis->m_pCallbacks.EventHandler(&pThis->m_cmp, pThis->m_app_data,
                                    OMX_EventCmdComplete,OMX_CommandStateSet,
                                    OMX_StateExecuting,NULL);
 }
 }

 break;

 case OMX_COMPONENT_GENERATE_STOP_DONE:
                    DEBUG_PRINT_LOW("OMX_COMPONENT_GENERATE_STOP_DONE msg");
 if (pThis->m_pCallbacks.EventHandler) {
                        pThis->complete_pending_buffer_done_cbs();
 if (BITMASK_PRESENT(&pThis->m_flags,OMX_COMPONENT_IDLE_PENDING)) {
                            BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_IDLE_PENDING);
                            pThis->m_state = OMX_StateIdle;
                            pThis->m_pCallbacks.EventHandler(&pThis->m_cmp,pThis->m_app_data,
                                    OMX_EventCmdComplete,OMX_CommandStateSet,
                                    OMX_StateIdle,NULL);
 } else if (BITMASK_PRESENT(&pThis->m_flags,
                                    OMX_COMPONENT_LOADED_STOP_PENDING)) {
 if (dev_loaded_stop_done()) {
                                DEBUG_PRINT_LOW("successful loaded Stop Done!");
 } else {
                                DEBUG_PRINT_ERROR("ERROR: failed in loaded Stop Done!");
                                pThis->omx_report_error ();
 }
                            BITMASK_CLEAR((&pThis->m_flags),OMX_COMPONENT_LOADED_STOP_PENDING);
 } else {
                            DEBUG_PRINT_LOW("ERROR: unknown flags=%" PRIx64, pThis->m_flags);
 }
 }

 break;

 case OMX_COMPONENT_GENERATE_HARDWARE_ERROR:
                    DEBUG_PRINT_ERROR("ERROR: OMX_COMPONENT_GENERATE_HARDWARE_ERROR!");
                    pThis->omx_report_error ();
 break;
#ifndef _MSM8974_
 case OMX_COMPONENT_GENERATE_LTRUSE_FAILED:
                    DEBUG_PRINT_ERROR("ERROR: OMX_COMPONENT_GENERATE_LTRUSE_FAILED!");
 if (pThis->m_pCallbacks.EventHandler) {
                        DEBUG_PRINT_ERROR("Sending QOMX_ErrorLTRUseFailed, p2 = 0x%x", p2);
                        pThis->m_pCallbacks.EventHandler(
 &pThis->m_cmp, pThis->m_app_data,
                                OMX_EventError, QOMX_ErrorLTRUseFailed, NULL, NULL);
 }
 break;
#endif
 default:
                    DEBUG_PRINT_LOW("process_event_cb unknown msg id 0x%02x", id);
 break;
 }
 }

        pthread_mutex_lock(&pThis->m_lock);
        qsize = pThis->m_cmd_q.m_size + pThis->m_ftb_q.m_size +\
                pThis->m_etb_q.m_size;

        pthread_mutex_unlock(&pThis->m_lock);

 } while (qsize>0);
    DEBUG_PRINT_LOW("exited the while loop");

}
