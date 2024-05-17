do_bundle_commit(struct ofconn *ofconn, uint32_t id, uint16_t flags)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    ovs_version_t version = ofproto->tables_version + 1;
    struct ofp_bundle *bundle;
    struct ofp_bundle_entry *be;
    enum ofperr error;

    bundle = ofconn_get_bundle(ofconn, id);

    if (!bundle) {
        return OFPERR_OFPBFC_BAD_ID;
    }
    if (bundle->flags != flags) {
        error = OFPERR_OFPBFC_BAD_FLAGS;
    } else {
        bool prev_is_port_mod = false;

        error = 0;
        ovs_mutex_lock(&ofproto_mutex);

         
        LIST_FOR_EACH (be, node, &bundle->msg_list) {
            if (be->type == OFPTYPE_PORT_MOD) {
                 
                if (flags & OFPBF_ATOMIC) {
                    error = OFPERR_OFPBFC_MSG_FAILED;
                } else {
                    prev_is_port_mod = true;
                    error = port_mod_start(ofconn, &be->opm.pm, &be->opm.port);
                }
            } else {
                 
                if (prev_is_port_mod) {
                    prev_is_port_mod = false;
                    ++version;
                }
                if (be->type == OFPTYPE_FLOW_MOD) {
                     
                    be->ofm.version = version;
                    error = ofproto_flow_mod_start(ofproto, &be->ofm);
                } else if (be->type == OFPTYPE_GROUP_MOD) {
                     
                    be->ogm.version = version;
                    error = ofproto_group_mod_start(ofproto, &be->ogm);
                } else if (be->type == OFPTYPE_PACKET_OUT) {
                    be->opo.version = version;
                    error = ofproto_packet_out_start(ofproto, &be->opo);
                } else {
                    OVS_NOT_REACHED();
                }
            }
            if (error) {
                break;
            }
        }

        if (error) {
             
            if (error) {
                ofconn_send_error(ofconn, &be->ofp_msg, error);
                error = OFPERR_OFPBFC_MSG_FAILED;
            }

             
            LIST_FOR_EACH_REVERSE_CONTINUE(be, node, &bundle->msg_list) {
                if (be->type == OFPTYPE_FLOW_MOD) {
                    ofproto_flow_mod_revert(ofproto, &be->ofm);
                } else if (be->type == OFPTYPE_GROUP_MOD) {
                    ofproto_group_mod_revert(ofproto, &be->ogm);
                } else if (be->type == OFPTYPE_PACKET_OUT) {
                    ofproto_packet_out_revert(ofproto, &be->opo);
                }
                 
            }
        } else {
             
            LIST_FOR_EACH (be, node, &bundle->msg_list) {
                if (be->type == OFPTYPE_PORT_MOD) {
                     
                    port_mod_finish(ofconn, &be->opm.pm, be->opm.port);
                } else {
                    version =
                        (be->type == OFPTYPE_FLOW_MOD) ? be->ofm.version :
                        (be->type == OFPTYPE_GROUP_MOD) ? be->ogm.version :
                        (be->type == OFPTYPE_PACKET_OUT) ? be->opo.version :
                        version;

                     
                    if (ofproto->tables_version < version) {
                        ofproto->tables_version = version;
                        ofproto->ofproto_class->set_tables_version(
                            ofproto, ofproto->tables_version);
                    }

                    struct openflow_mod_requester req = { ofconn,
                                                          &be->ofp_msg };

                    if (be->type == OFPTYPE_FLOW_MOD) {
                        ofproto_flow_mod_finish(ofproto, &be->ofm, &req);
                    } else if (be->type == OFPTYPE_GROUP_MOD) {
                        ofproto_group_mod_finish(ofproto, &be->ogm, &req);
                    } else if (be->type == OFPTYPE_PACKET_OUT) {
                        ofproto_packet_out_finish(ofproto, &be->opo);
                    }
                }
            }
        }

        ofmonitor_flush(ofproto->connmgr);
        ovs_mutex_unlock(&ofproto_mutex);
    }

     
    ofp_bundle_remove__(ofconn, bundle);
    return error;
}
