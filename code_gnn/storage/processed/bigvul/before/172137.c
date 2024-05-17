static void hal_says_data_ready(serial_data_type_t type) {
 packet_receive_data_t *incoming = &incoming_packets[PACKET_TYPE_TO_INBOUND_INDEX(type)];

 uint8_t byte;
 while (hal->read_data(type, &byte, 1, false) != 0) {
 switch (incoming->state) {
 case BRAND_NEW:
        incoming->bytes_remaining = preamble_sizes[PACKET_TYPE_TO_INDEX(type)];
        memset(incoming->preamble, 0, PREAMBLE_BUFFER_SIZE);
        incoming->index = 0;
        incoming->state = PREAMBLE;
 case PREAMBLE:
        incoming->preamble[incoming->index] = byte;
        incoming->index++;
        incoming->bytes_remaining--;

 if (incoming->bytes_remaining == 0) {
          incoming->bytes_remaining = (type == DATA_TYPE_ACL) ? RETRIEVE_ACL_LENGTH(incoming->preamble) : byte;

 size_t buffer_size = BT_HDR_SIZE + incoming->index + incoming->bytes_remaining;
          incoming->buffer = (BT_HDR *)buffer_allocator->alloc(buffer_size);

 if (!incoming->buffer) {
            LOG_ERROR("%s error getting buffer for incoming packet of type %d and size %zd", __func__, type, buffer_size);
            incoming->state = incoming->bytes_remaining == 0 ? BRAND_NEW : IGNORE;
 break;
 }

          incoming->buffer->offset = 0;
          incoming->buffer->layer_specific = 0;
          incoming->buffer->event = outbound_event_types[PACKET_TYPE_TO_INDEX(type)];
          memcpy(incoming->buffer->data, incoming->preamble, incoming->index);

          incoming->state = incoming->bytes_remaining > 0 ? BODY : FINISHED;
 }

 break;
 case BODY:
        incoming->buffer->data[incoming->index] = byte;
        incoming->index++;
        incoming->bytes_remaining--;

 size_t bytes_read = hal->read_data(type, (incoming->buffer->data + incoming->index), incoming->bytes_remaining, false);
        incoming->index += bytes_read;
        incoming->bytes_remaining -= bytes_read;

        incoming->state = incoming->bytes_remaining == 0 ? FINISHED : incoming->state;
 break;
 case IGNORE:
        incoming->bytes_remaining--;
 if (incoming->bytes_remaining == 0) {
          incoming->state = BRAND_NEW;
          hal->packet_finished(type);
 return;
 }

 break;
 case FINISHED:
        LOG_ERROR("%s the state machine should not have been left in the finished state.", __func__);
 break;
 }

 if (incoming->state == FINISHED) {
      incoming->buffer->len = incoming->index;
      btsnoop->capture(incoming->buffer, true);

 if (type != DATA_TYPE_EVENT) {
        packet_fragmenter->reassemble_and_dispatch(incoming->buffer);
 } else if (!filter_incoming_event(incoming->buffer)) {
 uint8_t *stream = incoming->buffer->data;
 uint8_t event_code;
        STREAM_TO_UINT8(event_code, stream);

        data_dispatcher_dispatch(
          interface.event_dispatcher,
          event_code,
          incoming->buffer
 );
 }

      incoming->buffer = NULL;
      incoming->state = BRAND_NEW;
      hal->packet_finished(type);

 return;
 }
 }
}
