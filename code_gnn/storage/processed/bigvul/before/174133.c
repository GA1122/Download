OMX_ERRORTYPE OMX::OnEvent(
        node_id node,
        OMX_IN OMX_EVENTTYPE eEvent,
        OMX_IN OMX_U32 nData1,
        OMX_IN OMX_U32 nData2,
        OMX_IN OMX_PTR pEventData) {
    ALOGV("OnEvent(%d, %" PRIu32", %" PRIu32 ")", eEvent, nData1, nData2);

    findInstance(node)->onEvent(eEvent, nData1, nData2);

    sp<OMX::CallbackDispatcher> dispatcher = findDispatcher(node);

 if (eEvent == OMX_EventOutputRendered) {
 if (pEventData == NULL) {
 return OMX_ErrorBadParameter;
 }

        OMX_VIDEO_RENDEREVENTTYPE *renderData = (OMX_VIDEO_RENDEREVENTTYPE *)pEventData;
 for (size_t i = 0; i < nData1; ++i) {
            omx_message msg;
            msg.type = omx_message::FRAME_RENDERED;
            msg.node = node;
            msg.fenceFd = -1;
            msg.u.render_data.timestamp = renderData[i].nMediaTimeUs;
            msg.u.render_data.nanoTime = renderData[i].nSystemTimeNs;

            dispatcher->post(msg, false  );
 }
 return OMX_ErrorNone;
 }

    omx_message msg;
    msg.type = omx_message::EVENT;
    msg.node = node;
    msg.fenceFd = -1;
    msg.u.event_data.event = eEvent;
    msg.u.event_data.data1 = nData1;
    msg.u.event_data.data2 = nData2;

    dispatcher->post(msg, true  );

 return OMX_ErrorNone;
}
