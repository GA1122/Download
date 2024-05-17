dissect_u3v_stream_trailer(proto_tree *u3v_telegram_tree, tvbuff_t *tvb, packet_info *pinfo, usb_conv_info_t *usb_conv_info _U_)
{
    gint offset = 0;
    guint64 block_id;
    proto_item *item = NULL;

     
    item = proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_trailer, tvb, 0, -1, ENC_NA);
    u3v_telegram_tree = proto_item_add_subtree(item, ett_u3v_stream_trailer);

     
    proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_prefix, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;

     
    proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_reserved, tvb, offset, 2, ENC_NA);
    offset += 2;

     
    proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_trailer_size, tvb, offset, 2, ENC_LITTLE_ENDIAN);
    offset += 2;

     
    block_id = tvb_get_letoh64(tvb, offset);
    proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_block_id, tvb, offset, 8, ENC_LITTLE_ENDIAN);
    offset += 8;

     
    proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_status, tvb, offset, 2, ENC_LITTLE_ENDIAN);
    offset += 2;

     
    proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_reserved, tvb, offset, 2, ENC_NA);
    offset += 2;

     
    proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_valid_payload_size, tvb, offset, 8, ENC_LITTLE_ENDIAN);
    offset += 8;

     
    col_append_fstr(pinfo->cinfo, COL_INFO, "Stream Trailer [ Block ID: %" G_GINT64_MODIFIER "u]", block_id);

    if (tvb_captured_length_remaining(tvb,offset) >=4 ) {
         
        proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_size_y, tvb, offset, 4, ENC_LITTLE_ENDIAN);
        offset += 4;
    }

    if (tvb_captured_length_remaining(tvb,offset) >=4 ) {
         
        proto_tree_add_item(u3v_telegram_tree, hf_u3v_stream_chunk_layout_id, tvb, offset, 4, ENC_LITTLE_ENDIAN);
         
    }
}
