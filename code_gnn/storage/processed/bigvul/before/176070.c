static uint8_t* add_attr(uint8_t* p, tSDP_DISCOVERY_DB* p_db,
                         tSDP_DISC_REC* p_rec, uint16_t attr_id,
                         tSDP_DISC_ATTR* p_parent_attr, uint8_t nest_level) {
  tSDP_DISC_ATTR* p_attr;
 uint32_t attr_len;
 uint32_t total_len;
 uint16_t attr_type;
 uint16_t id;
 uint8_t type;
 uint8_t* p_end;
 uint8_t is_additional_list = nest_level & SDP_ADDITIONAL_LIST_MASK;

  nest_level &= ~(SDP_ADDITIONAL_LIST_MASK);

  type = *p++;
  p = sdpu_get_len_from_type(p, type, &attr_len);

  attr_len &= SDP_DISC_ATTR_LEN_MASK;
  attr_type = (type >> 3) & 0x0f;

  
 if (attr_len > 4)
    total_len = attr_len - 4 + (uint16_t)sizeof(tSDP_DISC_ATTR);
 else
    total_len = sizeof(tSDP_DISC_ATTR);

  
  total_len = (total_len + 3) & ~3;

  
 if (p_db->mem_free < total_len) return (NULL);

  p_attr = (tSDP_DISC_ATTR*)p_db->p_free_mem;
  p_attr->attr_id = attr_id;
  p_attr->attr_len_type = (uint16_t)attr_len | (attr_type << 12);
  p_attr->p_next_attr = NULL;

  
 switch (attr_type) {
 case UINT_DESC_TYPE:
 if ((is_additional_list != 0) && (attr_len == 2)) {
        BE_STREAM_TO_UINT16(id, p);
 if (id != ATTR_ID_PROTOCOL_DESC_LIST)
          p -= 2;
 else {
  
          p_db->p_free_mem += sizeof(tSDP_DISC_ATTR);
          p_db->mem_free -= sizeof(tSDP_DISC_ATTR);
          p_end = p + attr_len;
          total_len = 0;

  
 if (nest_level >= MAX_NEST_LEVELS) {
            SDP_TRACE_ERROR("SDP - attr nesting too deep");
 return (p_end);
 }

  
          p = add_attr(p, p_db, p_rec, ATTR_ID_PROTOCOL_DESC_LIST, p_attr,
 (uint8_t)(nest_level + 1));

 break;
 }
 }
  

 case TWO_COMP_INT_DESC_TYPE:
 switch (attr_len) {
 case 1:
          p_attr->attr_value.v.u8 = *p++;
 break;
 case 2:
          BE_STREAM_TO_UINT16(p_attr->attr_value.v.u16, p);
 break;
 case 4:
          BE_STREAM_TO_UINT32(p_attr->attr_value.v.u32, p);
 break;
 default:
          BE_STREAM_TO_ARRAY(p, p_attr->attr_value.v.array, (int32_t)attr_len);
 break;
 }
 break;

 case UUID_DESC_TYPE:
 switch (attr_len) {
 case 2:
          BE_STREAM_TO_UINT16(p_attr->attr_value.v.u16, p);
 break;
 case 4:
          BE_STREAM_TO_UINT32(p_attr->attr_value.v.u32, p);
 if (p_attr->attr_value.v.u32 < 0x10000) {
            attr_len = 2;
            p_attr->attr_len_type = (uint16_t)attr_len | (attr_type << 12);
            p_attr->attr_value.v.u16 = (uint16_t)p_attr->attr_value.v.u32;
 }
 break;
 case 16:
  
 if (sdpu_is_base_uuid(p)) {
 if ((p[0] == 0) && (p[1] == 0)) {
              p_attr->attr_len_type =
 (p_attr->attr_len_type & ~SDP_DISC_ATTR_LEN_MASK) | 2;
              p += 2;
              BE_STREAM_TO_UINT16(p_attr->attr_value.v.u16, p);
              p += Uuid::kNumBytes128 - 4;
 } else {
              p_attr->attr_len_type =
 (p_attr->attr_len_type & ~SDP_DISC_ATTR_LEN_MASK) | 4;
              BE_STREAM_TO_UINT32(p_attr->attr_value.v.u32, p);
              p += Uuid::kNumBytes128 - 4;
 }
 } else {
            BE_STREAM_TO_ARRAY(p, p_attr->attr_value.v.array,
 (int32_t)attr_len);
 }
 break;
 default:
          SDP_TRACE_WARNING("SDP - bad len in UUID attr: %d", attr_len);
 return (p + attr_len);
 }
 break;

 case DATA_ELE_SEQ_DESC_TYPE:
 case DATA_ELE_ALT_DESC_TYPE:
  
      p_db->p_free_mem += sizeof(tSDP_DISC_ATTR);
      p_db->mem_free -= sizeof(tSDP_DISC_ATTR);
      p_end = p + attr_len;
      total_len = 0;

  
 if (nest_level >= MAX_NEST_LEVELS) {
        SDP_TRACE_ERROR("SDP - attr nesting too deep");
 return (p_end);
 }
 if (is_additional_list != 0 ||
          attr_id == ATTR_ID_ADDITION_PROTO_DESC_LISTS)
        nest_level |= SDP_ADDITIONAL_LIST_MASK;
  

 while (p < p_end) {
  
        p = add_attr(p, p_db, p_rec, 0, p_attr, (uint8_t)(nest_level + 1));

 if (!p) return (NULL);
 }
 break;

 case TEXT_STR_DESC_TYPE:
 case URL_DESC_TYPE:
      BE_STREAM_TO_ARRAY(p, p_attr->attr_value.v.array, (int32_t)attr_len);
 break;

 case BOOLEAN_DESC_TYPE:
 switch (attr_len) {
 case 1:
          p_attr->attr_value.v.u8 = *p++;
 break;
 default:
          SDP_TRACE_WARNING("SDP - bad len in boolean attr: %d", attr_len);
 return (p + attr_len);
 }
 break;

 default:  
 break;
 }

  p_db->p_free_mem += total_len;
  p_db->mem_free -= total_len;

  
 if (!p_parent_attr) {
 if (!p_rec->p_first_attr)
      p_rec->p_first_attr = p_attr;
 else {
      tSDP_DISC_ATTR* p_attr1 = p_rec->p_first_attr;

 while (p_attr1->p_next_attr) p_attr1 = p_attr1->p_next_attr;

      p_attr1->p_next_attr = p_attr;
 }
 } else {
 if (!p_parent_attr->attr_value.v.p_sub_attr) {
      p_parent_attr->attr_value.v.p_sub_attr = p_attr;
  
 } else {
      tSDP_DISC_ATTR* p_attr1 = p_parent_attr->attr_value.v.p_sub_attr;
  

 while (p_attr1->p_next_attr) p_attr1 = p_attr1->p_next_attr;

      p_attr1->p_next_attr = p_attr;
  
 }
 }

 return (p);
}
