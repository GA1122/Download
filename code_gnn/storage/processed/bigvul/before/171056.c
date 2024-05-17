void RilSapSocket::pushRecord(void *p_record, size_t recordlen) {
 pb_istream_t stream = pb_istream_from_buffer((uint8_t *)p_record, recordlen);
 MsgHeader *reqHeader = (MsgHeader *)malloc(sizeof (MsgHeader));
 if (!reqHeader) {
        RLOGE("pushRecord: OOM");
 return;
 }
    memset(reqHeader, 0, sizeof(MsgHeader));

    log_hex("BtSapTest-Payload", (const uint8_t*)p_record, recordlen);

 if (!pb_decode(&stream, MsgHeader_fields, reqHeader) ) {
        RLOGE("Error decoding protobuf buffer : %s", PB_GET_ERROR(&stream));
        free(reqHeader);
 } else {
 SapSocketRequest *recv = (SapSocketRequest*)malloc(sizeof(SapSocketRequest));
 if (!recv) {
            RLOGE("pushRecord: OOM");
            free(reqHeader);
 return;
 }
        recv->token = reqHeader->token;
        recv->curr = reqHeader;
        recv->socketId = id;

        dispatchQueue.enqueue(recv);
 }
}
