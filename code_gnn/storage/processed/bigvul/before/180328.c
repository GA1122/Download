 fp_set_per_packet_inf_from_conv(umts_fp_conversation_info_t *p_conv_data,
                                 tvbuff_t *tvb, packet_info *pinfo,
                                 proto_tree *tree _U_)
 {
     fp_info  *fpi;
     guint8    tfi, c_t;
     int       offset = 0, i=0, j=0, num_tbs, chan, tb_size, tb_bit_off;
     gboolean  is_control_frame;
     umts_mac_info *macinf;
     rlc_info *rlcinf;
     guint8 fake_lchid=0;
     gint *cur_val=NULL;
 
     fpi = wmem_new0(wmem_file_scope(), fp_info);
     p_add_proto_data(wmem_file_scope(), pinfo, proto_fp, 0, fpi);
 
     fpi->iface_type = p_conv_data->iface_type;
     fpi->division = p_conv_data->division;
     fpi->release = 7;                
     fpi->release_year = 2006;
     fpi->release_month = 12;
     fpi->channel = p_conv_data->channel;
     fpi->dch_crc_present = p_conv_data->dch_crc_present;
      
     fpi->link_type = FP_Link_Ethernet;
 
 #if 0
      
     if (!pinfo->fd->flags.visited &&  p_conv_data->reset_frag ) {
         fpi->reset_frag = p_conv_data->reset_frag;
         p_conv_data->reset_frag = FALSE;
     }
 #endif
      
     fpi->srcport = pinfo->srcport;
     fpi->destport = pinfo->destport;
 
     fpi->com_context_id = p_conv_data->com_context_id;
 
     if (pinfo->link_dir == P2P_DIR_UL) {
         fpi->is_uplink = TRUE;
     } else {
         fpi->is_uplink = FALSE;
     }
 
     is_control_frame = tvb_get_guint8(tvb, offset) & 0x01;
 
     switch (fpi->channel) {
         case CHANNEL_HSDSCH:  
             fpi->hsdsch_entity = p_conv_data->hsdsch_entity;
             macinf = wmem_new0(wmem_file_scope(), umts_mac_info);
             fpi->hsdsch_macflowd_id = p_conv_data->hsdsch_macdflow_id;
            macinf->content[0] = hsdsch_macdflow_id_mac_content_map[p_conv_data->hsdsch_macdflow_id];  
             macinf->lchid[0] = p_conv_data->hsdsch_macdflow_id;
              
             p_add_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0, macinf);
 
             rlcinf = wmem_new0(wmem_file_scope(), rlc_info);
 
              
             rlcinf->mode[0] = hsdsch_macdflow_id_rlc_map[p_conv_data->hsdsch_macdflow_id];
 
             if (fpi->hsdsch_entity == hs  ) {
                 for (i=0; i<MAX_NUM_HSDHSCH_MACDFLOW; i++) {
                      
                     if ((cur_val=(gint *)g_tree_lookup(hsdsch_muxed_flows, GINT_TO_POINTER((gint)p_conv_data->hrnti))) != NULL) {
                         j = 1 << i;
                         fpi->hsdhsch_macfdlow_is_mux[i] = j & *cur_val;
                     } else {
                         fpi->hsdhsch_macfdlow_is_mux[i] = FALSE;
                     }
 
                 }
             }
              
              
              
              
              
             rlcinf->urnti[0] = fpi->com_context_id;
             rlcinf->li_size[0] = RLC_LI_7BITS;
             rlcinf->ciphered[0] = FALSE;
             rlcinf->deciphered[0] = FALSE;
             p_add_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0, rlcinf);
 
 
             return fpi;
 
         case CHANNEL_EDCH:
              
             macinf = wmem_new0(wmem_file_scope(), umts_mac_info);
             rlcinf = wmem_new0(wmem_file_scope(), rlc_info);
             fpi->no_ddi_entries = p_conv_data->no_ddi_entries;
             for (i=0; i<fpi->no_ddi_entries; i++) {
                 fpi->edch_ddi[i] = p_conv_data->edch_ddi[i];     
                 fpi->edch_macd_pdu_size[i] = p_conv_data->edch_macd_pdu_size[i];     
                 fpi->edch_lchId[i] = p_conv_data->edch_lchId[i];     
                       
                
 
             }
             fpi->edch_type = p_conv_data->edch_type;
 
             
             p_add_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0, macinf);
 
 
              
             rlcinf->urnti[0] = fpi->com_context_id;
             
             rlcinf->li_size[0] = RLC_LI_7BITS;
             rlcinf->ciphered[0] = FALSE;
             rlcinf->deciphered[0] = FALSE;
 
             p_add_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0, rlcinf);
 
             return fpi;
 
         case CHANNEL_PCH:
             fpi->paging_indications = p_conv_data->paging_indications;
             fpi->num_chans = p_conv_data->num_dch_in_flow;
              
             if (is_control_frame) {
                  
                 return fpi;
             }
              
             offset = 3;
             break;
         case CHANNEL_DCH:
             fpi->num_chans = p_conv_data->num_dch_in_flow;
             if (is_control_frame) {
                  
                 return fpi;
             }
 
             rlcinf = wmem_new0(wmem_file_scope(), rlc_info);
             macinf = wmem_new0(wmem_file_scope(), umts_mac_info);
             offset = 2;     
             fakes  = 5;  
             for (chan=0; chan < fpi->num_chans; chan++) {     
                      
                     
                     
                     tfi = tvb_get_bits8(tvb, 3+offset*8, 5);
 
                     
                    num_tbs = (fpi->is_uplink) ? p_conv_data->fp_dch_channel_info[chan].ul_chan_num_tbs[tfi] : p_conv_data->fp_dch_channel_info[chan].dl_chan_num_tbs[tfi];
                    tb_size=  (fpi->is_uplink) ? p_conv_data->fp_dch_channel_info[i].ul_chan_tf_size[tfi] :    p_conv_data->fp_dch_channel_info[i].dl_chan_tf_size[tfi];
 
                      
                      
                     if ( p_conv_data->dchs_in_flow_list[chan] != 31 && (p_conv_data->dchs_in_flow_list[chan] == 24 &&
                      tb_size != 340) ) {
                         fake_lchid = make_fake_lchid(pinfo, p_conv_data->dchs_in_flow_list[chan]);
                     }
                     tb_bit_off = (2+p_conv_data->num_dch_in_flow)*8;     
                      
                     for (j=0; j < num_tbs && j+chan < MAX_MAC_FRAMES; j++) {
                          
                         macinf->trchid[j+chan] = p_conv_data->dchs_in_flow_list[chan];
 
                          
                         if ( p_conv_data->dchs_in_flow_list[chan] == 31 || p_conv_data->dchs_in_flow_list[chan] == 24) {
 
                              
                              
                             if (0 ) {
                                 macinf->ctmux[j+chan] = FALSE; 
 
                                 macinf->lchid[j+chan] = 1;
 
                                 macinf->content[j+chan] = lchId_type_table[1];     
                                 rlcinf->mode[j+chan] = lchId_rlc_map[1];     
 
                             }
                              
                             else if (p_conv_data->dchs_in_flow_list[chan] == 24 && tb_size != 340) {
                                 macinf->ctmux[j+chan] = FALSE; 
 
                                  
                                 macinf->lchid[j+chan] = fake_lchid;
                                 macinf->fake_chid[j+chan] = TRUE;
                                 macinf->content[j+chan] = MAC_CONTENT_PS_DTCH;       
                                 rlcinf->mode[j+chan] = RLC_AM;      
                             }
                              
                             else {
                                 macinf->ctmux[j+chan] = TRUE; 
  
                                   
                                   
                                c_t = tvb_get_bits8(tvb, tb_bit_off , 4);     
                                macinf->lchid[j+chan] = c_t+1;
//                                 c_t = (tvb_get_bits8(tvb, tb_bit_off , 4) + 1) % 0xf;     
//                                 macinf->lchid[j+chan] = c_t;
  
                                macinf->content[j+chan] = lchId_type_table[c_t+1];     
                                rlcinf->mode[j+chan] = lchId_rlc_map[c_t+1];     
//                                 macinf->content[j+chan] = lchId_type_table[c_t];     
//                                 rlcinf->mode[j+chan] = lchId_rlc_map[c_t];     
                              }
                          } else {
                              fake_lchid = make_fake_lchid(pinfo, p_conv_data->dchs_in_flow_list[chan]);
                             macinf->ctmux[j+chan] = FALSE; 
                              
                             macinf->content[j+chan] = lchId_type_table[fake_lchid];
 
 
                             rlcinf->mode[j+chan] = lchId_rlc_map[fake_lchid];
 
                              
                              
                              
                             macinf->fake_chid[j+chan] = TRUE;
                             macinf->lchid[j+chan] = fake_lchid;   
                              
                         }
 
                          
                         rlcinf->urnti[j+chan] = p_conv_data->com_context_id;
                         rlcinf->li_size[j+chan] = RLC_LI_7BITS;
 #if 0
                          
                         if ( rrc_ciph_inf && g_tree_lookup(rrc_ciph_inf, GINT_TO_POINTER((gint)p_conv_data->com_context_id)) != NULL ) {
                             rlcinf->ciphered[j+chan] = TRUE;
                         } else {
                             rlcinf->ciphered[j+chan] = FALSE;
                         }
 #endif
                         rlcinf->ciphered[j+chan] = FALSE;
                         rlcinf->deciphered[j+chan] = FALSE;
                         rlcinf->rbid[j+chan] = macinf->lchid[j+chan];
 
 
                          
                         tb_bit_off += tb_size+4;
                     }
 
                     offset++;
             }
             p_add_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0, macinf);
             p_add_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0, rlcinf);
              
             offset = 2;
             break;
         case CHANNEL_FACH_FDD:
             fpi->num_chans = p_conv_data->num_dch_in_flow;
             if (is_control_frame) {
                  
                 return fpi;
             }
              
             offset = 2;
              
             macinf = wmem_new0(wmem_file_scope(), umts_mac_info);
             macinf->ctmux[0]   = 1;
             macinf->content[0] = MAC_CONTENT_DCCH;
             p_add_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0, macinf);
              
             rlcinf = wmem_new0(wmem_file_scope(), rlc_info);
              
              
             rlcinf->urnti[0] = fpi->channel;
             rlcinf->mode[0] = RLC_AM;
              
             rlcinf->li_size[0] = RLC_LI_7BITS;
             rlcinf->ciphered[0] = FALSE;
             rlcinf->deciphered[0] = FALSE;
             p_add_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0, rlcinf);
             break;
 
         case CHANNEL_RACH_FDD:
             fpi->num_chans = p_conv_data->num_dch_in_flow;
             if (is_control_frame) {
                  
                 return fpi;
             }
              
             offset = 2;
              
             macinf = wmem_new0(wmem_file_scope(), umts_mac_info);
             rlcinf = wmem_new0(wmem_file_scope(), rlc_info);
             for ( chan = 0; chan < fpi->num_chans; chan++ ) {
                     macinf->ctmux[chan]   = 1;
                     macinf->content[chan] = MAC_CONTENT_DCCH;
                     rlcinf->urnti[chan] = fpi->com_context_id;     
             }
 
 
 
             p_add_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0, macinf);
             p_add_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0, rlcinf);
             break;
         case CHANNEL_HSDSCH_COMMON:
                 rlcinf = wmem_new0(wmem_file_scope(), rlc_info);
                 macinf = wmem_new0(wmem_file_scope(), umts_mac_info);
                 p_add_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0, macinf);
                 p_add_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0, rlcinf);
             break;
         default:
             expert_add_info(pinfo, NULL, &ei_fp_transport_channel_type_unknown);
             return NULL;
     }
 
      
     for (i=0; i<fpi->num_chans; i++) {
         tfi = tvb_get_guint8(tvb, offset);
 
          
          
         if (pinfo->link_dir == P2P_DIR_UL) {
             fpi->chan_tf_size[i] = p_conv_data->fp_dch_channel_info[i].ul_chan_tf_size[tfi];
             fpi->chan_num_tbs[i] = p_conv_data->fp_dch_channel_info[i].ul_chan_num_tbs[tfi];
         } else {
             fpi->chan_tf_size[i] = p_conv_data->fp_dch_channel_info[i].dl_chan_tf_size[tfi];
             fpi->chan_num_tbs[i] = p_conv_data->fp_dch_channel_info[i].dl_chan_num_tbs[tfi];
         }
         offset++;
     }
 
 
     return fpi;
 }