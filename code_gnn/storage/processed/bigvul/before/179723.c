 static int scsi_disk_emulate_command(SCSIDiskReq *r, uint8_t *outbuf)
// static int scsi_disk_emulate_command(SCSIDiskReq *r)
  {
      SCSIRequest *req = &r->req;
      SCSIDiskState *s = DO_UPCAST(SCSIDiskState, qdev, req->dev);
      uint64_t nb_sectors;
//     uint8_t *outbuf;
      int buflen = 0;
  
//     if (!r->iov.iov_base) {
//          
//         if (req->cmd.xfer > 65536) {
//             goto illegal_request;
//         }
//         r->buflen = MAX(4096, req->cmd.xfer);
//         r->iov.iov_base = qemu_blockalign(s->bs, r->buflen);
//     }
// 
//     outbuf = r->iov.iov_base;
      switch (req->cmd.buf[0]) {
      case TEST_UNIT_READY:
          if (s->tray_open || !bdrv_is_inserted(s->bs))
             goto not_ready;
         break;
     case INQUIRY:
         buflen = scsi_disk_emulate_inquiry(req, outbuf);
         if (buflen < 0)
             goto illegal_request;
         break;
     case MODE_SENSE:
     case MODE_SENSE_10:
         buflen = scsi_disk_emulate_mode_sense(r, outbuf);
         if (buflen < 0)
             goto illegal_request;
         break;
     case READ_TOC:
         buflen = scsi_disk_emulate_read_toc(req, outbuf);
         if (buflen < 0)
             goto illegal_request;
         break;
     case RESERVE:
         if (req->cmd.buf[1] & 1)
             goto illegal_request;
         break;
     case RESERVE_10:
         if (req->cmd.buf[1] & 3)
             goto illegal_request;
         break;
     case RELEASE:
         if (req->cmd.buf[1] & 1)
             goto illegal_request;
         break;
     case RELEASE_10:
         if (req->cmd.buf[1] & 3)
             goto illegal_request;
         break;
     case START_STOP:
         if (scsi_disk_emulate_start_stop(r) < 0) {
             return -1;
         }
         break;
     case ALLOW_MEDIUM_REMOVAL:
         s->tray_locked = req->cmd.buf[4] & 1;
         bdrv_lock_medium(s->bs, req->cmd.buf[4] & 1);
         break;
     case READ_CAPACITY_10:
          
         memset(outbuf, 0, 8);
         bdrv_get_geometry(s->bs, &nb_sectors);
         if (!nb_sectors)
             goto not_ready;
         nb_sectors /= s->cluster_size;
          
         nb_sectors--;
          
         s->max_lba = nb_sectors;
          
         if (nb_sectors > UINT32_MAX)
             nb_sectors = UINT32_MAX;
         outbuf[0] = (nb_sectors >> 24) & 0xff;
         outbuf[1] = (nb_sectors >> 16) & 0xff;
         outbuf[2] = (nb_sectors >> 8) & 0xff;
         outbuf[3] = nb_sectors & 0xff;
         outbuf[4] = 0;
         outbuf[5] = 0;
         outbuf[6] = s->cluster_size * 2;
         outbuf[7] = 0;
         buflen = 8;
         break;
     case GET_CONFIGURATION:
         memset(outbuf, 0, 8);
          
         outbuf[7] = 8;  
         buflen = 8;
         break;
     case SERVICE_ACTION_IN_16:
          
         if ((req->cmd.buf[1] & 31) == SAI_READ_CAPACITY_16) {
             DPRINTF("SAI READ CAPACITY(16)\n");
             memset(outbuf, 0, req->cmd.xfer);
             bdrv_get_geometry(s->bs, &nb_sectors);
             if (!nb_sectors)
                 goto not_ready;
             nb_sectors /= s->cluster_size;
              
             nb_sectors--;
              
             s->max_lba = nb_sectors;
             outbuf[0] = (nb_sectors >> 56) & 0xff;
             outbuf[1] = (nb_sectors >> 48) & 0xff;
             outbuf[2] = (nb_sectors >> 40) & 0xff;
             outbuf[3] = (nb_sectors >> 32) & 0xff;
             outbuf[4] = (nb_sectors >> 24) & 0xff;
             outbuf[5] = (nb_sectors >> 16) & 0xff;
             outbuf[6] = (nb_sectors >> 8) & 0xff;
             outbuf[7] = nb_sectors & 0xff;
             outbuf[8] = 0;
             outbuf[9] = 0;
             outbuf[10] = s->cluster_size * 2;
             outbuf[11] = 0;
             outbuf[12] = 0;
             outbuf[13] = get_physical_block_exp(&s->qdev.conf);
 
              
             if (s->qdev.conf.discard_granularity) {
                 outbuf[14] = 0x80;
             }
 
              
             buflen = req->cmd.xfer;
             break;
         }
         DPRINTF("Unsupported Service Action In\n");
         goto illegal_request;
     case VERIFY_10:
         break;
     default:
         scsi_check_condition(r, SENSE_CODE(INVALID_OPCODE));
         return -1;
     }
     return buflen;
 
 not_ready:
     if (s->tray_open || !bdrv_is_inserted(s->bs)) {
         scsi_check_condition(r, SENSE_CODE(NO_MEDIUM));
     } else {
         scsi_check_condition(r, SENSE_CODE(LUN_NOT_READY));
     }
     return -1;
 
 illegal_request:
     if (r->req.status == -1) {
         scsi_check_condition(r, SENSE_CODE(INVALID_FIELD));
     }
     return -1;
 }