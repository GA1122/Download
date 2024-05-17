static void lsi_do_msgout(LSIState *s)
{
    uint8_t msg;
    int len;
    uint32_t current_tag;
    lsi_request *current_req, *p, *p_next;

    if (s->current) {
        current_tag = s->current->tag;
        current_req = s->current;
    } else {
        current_tag = s->select_tag;
        current_req = lsi_find_by_tag(s, current_tag);
    }

    trace_lsi_do_msgout(s->dbc);
    while (s->dbc) {
        msg = lsi_get_msgbyte(s);
        s->sfbr = msg;

        switch (msg) {
        case 0x04:
            trace_lsi_do_msgout_disconnect();
            lsi_disconnect(s);
            break;
        case 0x08:
            trace_lsi_do_msgout_noop();
            lsi_set_phase(s, PHASE_CMD);
            break;
        case 0x01:
            len = lsi_get_msgbyte(s);
            msg = lsi_get_msgbyte(s);
            (void)len;  
            trace_lsi_do_msgout_extended(msg, len);
            switch (msg) {
            case 1:
                trace_lsi_do_msgout_ignored("SDTR");
                lsi_skip_msgbytes(s, 2);
                break;
            case 3:
                trace_lsi_do_msgout_ignored("WDTR");
                lsi_skip_msgbytes(s, 1);
                break;
            case 4:
                trace_lsi_do_msgout_ignored("PPR");
                lsi_skip_msgbytes(s, 5);
                break;
            default:
                goto bad;
            }
            break;
        case 0x20:  
            s->select_tag |= lsi_get_msgbyte(s) | LSI_TAG_VALID;
            trace_lsi_do_msgout_simplequeue(s->select_tag & 0xff);
            break;
        case 0x21:  
            qemu_log_mask(LOG_UNIMP, "lsi_scsi: HEAD queue not implemented\n");
            s->select_tag |= lsi_get_msgbyte(s) | LSI_TAG_VALID;
            break;
        case 0x22:  
            qemu_log_mask(LOG_UNIMP,
                          "lsi_scsi: ORDERED queue not implemented\n");
            s->select_tag |= lsi_get_msgbyte(s) | LSI_TAG_VALID;
            break;
        case 0x0d:
             
            trace_lsi_do_msgout_abort(current_tag);
            if (current_req) {
                scsi_req_cancel(current_req->req);
            }
            lsi_disconnect(s);
            break;
        case 0x06:
        case 0x0e:
        case 0x0c:
             
            if (msg == 0x06) {
                trace_lsi_do_msgout_abort(current_tag);
            }
             
            if (msg == 0x0e) {
                trace_lsi_do_msgout_clearqueue(current_tag);
            }
             
            if (msg == 0x0c) {
                trace_lsi_do_msgout_busdevicereset(current_tag);
            }

             
            if (s->current) {
                scsi_req_cancel(s->current->req);
            }

             
            QTAILQ_FOREACH_SAFE(p, &s->queue, next, p_next) {
                if ((p->tag & 0x0000ff00) == (current_tag & 0x0000ff00)) {
                    scsi_req_cancel(p->req);
                }
            }

            lsi_disconnect(s);
            break;
        default:
            if ((msg & 0x80) == 0) {
                goto bad;
            }
            s->current_lun = msg & 7;
            trace_lsi_do_msgout_select(s->current_lun);
            lsi_set_phase(s, PHASE_CMD);
            break;
        }
    }
    return;
bad:
    qemu_log_mask(LOG_UNIMP, "Unimplemented message 0x%02x\n", msg);
    lsi_set_phase(s, PHASE_MI);
    lsi_add_msg_byte(s, 7);  
    s->msg_action = LSI_MSG_ACTION_COMMAND;
}
