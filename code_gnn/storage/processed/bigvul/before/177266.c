 virtual void onMessages(const std::list<omx_message> &messages) {
 if (messages.empty()) {
 return;
 }

        sp<AMessage> notify = mNotify->dup();
 bool first = true;
        sp<MessageList> msgList = new MessageList();
 for (std::list<omx_message>::const_iterator it = messages.cbegin();
              it != messages.cend(); ++it) {
 const omx_message &omx_msg = *it;
 if (first) {
                notify->setInt32("node", omx_msg.node);
                first = false;
 }

            sp<AMessage> msg = new AMessage;
            msg->setInt32("type", omx_msg.type);
 switch (omx_msg.type) {
 case omx_message::EVENT:
 {
                    msg->setInt32("event", omx_msg.u.event_data.event);
                    msg->setInt32("data1", omx_msg.u.event_data.data1);
                    msg->setInt32("data2", omx_msg.u.event_data.data2);
 break;
 }

 case omx_message::EMPTY_BUFFER_DONE:
 {
                    msg->setInt32("buffer", omx_msg.u.buffer_data.buffer);
                    msg->setInt32("fence_fd", omx_msg.fenceFd);
 break;
 }

 case omx_message::FILL_BUFFER_DONE:
 {
                    msg->setInt32(
 "buffer", omx_msg.u.extended_buffer_data.buffer);
                    msg->setInt32(
 "range_offset",
                            omx_msg.u.extended_buffer_data.range_offset);
                    msg->setInt32(
 "range_length",
                            omx_msg.u.extended_buffer_data.range_length);
                    msg->setInt32(
 "flags",
                            omx_msg.u.extended_buffer_data.flags);
                    msg->setInt64(
 "timestamp",
                            omx_msg.u.extended_buffer_data.timestamp);
                    msg->setInt32(
 "fence_fd", omx_msg.fenceFd);
 break;
 }

 case omx_message::FRAME_RENDERED:
 {
                    msg->setInt64(
 "media_time_us", omx_msg.u.render_data.timestamp);
                    msg->setInt64(
 "system_nano", omx_msg.u.render_data.nanoTime);
 break;
 }

 default:
                    ALOGE("Unrecognized message type: %d", omx_msg.type);
 break;
 }
            msgList->getList().push_back(msg);
 }
        notify->setObject("messages", msgList);
        notify->post();
 }
