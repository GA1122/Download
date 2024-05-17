static uint8_t* save_attr_seq(tCONN_CB* p_ccb, uint8_t* p, uint8_t* p_msg_end) {
 uint32_t seq_len, attr_len;
 uint16_t attr_id;
 uint8_t type, *p_seq_end;
  tSDP_DISC_REC* p_rec;

  type = *p++;

 if ((type >> 3) != DATA_ELE_SEQ_DESC_TYPE) {
    SDP_TRACE_WARNING("SDP - Wrong type: 0x%02x in attr_rsp", type);
 return (NULL);
 }

  p = sdpu_get_len_from_type(p, type, &seq_len);
 if ((p + seq_len) > p_msg_end) {
    SDP_TRACE_WARNING("SDP - Bad len in attr_rsp %d", seq_len);
 return (NULL);
 }

  
  p_rec = add_record(p_ccb->p_db, p_ccb->device_address);
 if (!p_rec) {
    SDP_TRACE_WARNING("SDP - DB full add_record");
 return (NULL);
 }

  p_seq_end = p + seq_len;

 while (p < p_seq_end) {
  
    type = *p++;
    p = sdpu_get_len_from_type(p, type, &attr_len);
 if (((type >> 3) != UINT_DESC_TYPE) || (attr_len != 2)) {
      SDP_TRACE_WARNING("SDP - Bad type: 0x%02x or len: %d in attr_rsp", type,
                        attr_len);
 return (NULL);
 }
    BE_STREAM_TO_UINT16(attr_id, p);

  
    p = add_attr(p, p_ccb->p_db, p_rec, attr_id, NULL, 0);

 if (!p) {
      SDP_TRACE_WARNING("SDP - DB full add_attr");
 return (NULL);
 }
 }

 return (p);
}
