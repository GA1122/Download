heur_dissect_fp(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
    struct fp_info *p_fp_info;

    p_fp_info = (fp_info *)p_get_proto_data(wmem_file_scope(), pinfo, proto_fp, 0);

     
    if (!p_fp_info) {
         
        int offset = 0, length;
        guint8 oct, calc_crc = 0, crc;
        unsigned char *buf;

        oct = tvb_get_guint8(tvb, offset);
        crc = oct & 0xfe;
        if ((oct & 0x01) == 1) {
             
            length =  tvb_reported_length(tvb);
            buf = (unsigned char *)tvb_memdup(wmem_packet_scope(), tvb, 0, length);
            buf[0] = 01;

            calc_crc = crc7update(calc_crc, buf, length);
            if (calc_crc == crc) {
                 
                conversation_set_dissector(find_or_create_conversation(pinfo), fp_handle);
                dissect_fp(tvb, pinfo, tree, data);
                return TRUE;
            }
        }
        return FALSE;
    }

     
    if (p_fp_info->link_type != FP_Link_Ethernet) {
        return FALSE;
    }

     
    if (p_fp_info->srcport != pinfo->srcport ||
        p_fp_info->destport != pinfo->destport)
        return FALSE;

     
    dissect_fp(tvb, pinfo, tree, data);
    return TRUE;
}
