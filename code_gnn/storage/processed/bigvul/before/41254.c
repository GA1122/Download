static int scsi_disk_emulate_inquiry(SCSIRequest *req, uint8_t *outbuf)
{
    SCSIDiskState *s = DO_UPCAST(SCSIDiskState, qdev, req->dev);
    int buflen = 0;

    if (req->cmd.buf[1] & 0x2) {
         
        BADF("optional INQUIRY command support request not implemented\n");
        return -1;
    }

    if (req->cmd.buf[1] & 0x1) {
         
        uint8_t page_code = req->cmd.buf[2];
        if (req->cmd.xfer < 4) {
            BADF("Error: Inquiry (EVPD[%02X]) buffer size %zd is "
                 "less than 4\n", page_code, req->cmd.xfer);
            return -1;
        }

        if (s->qdev.type == TYPE_ROM) {
            outbuf[buflen++] = 5;
        } else {
            outbuf[buflen++] = 0;
        }
        outbuf[buflen++] = page_code ;  
        outbuf[buflen++] = 0x00;

        switch (page_code) {
        case 0x00:  
        {
            int pages;
            DPRINTF("Inquiry EVPD[Supported pages] "
                    "buffer size %zd\n", req->cmd.xfer);
            pages = buflen++;
            outbuf[buflen++] = 0x00;  
            if (s->serial)
                outbuf[buflen++] = 0x80;  
            outbuf[buflen++] = 0x83;  
            if (s->qdev.type == TYPE_DISK) {
                outbuf[buflen++] = 0xb0;  
                outbuf[buflen++] = 0xb2;  
            }
            outbuf[pages] = buflen - pages - 1;  
            break;
        }
        case 0x80:  
        {
            int l;

            if (!s->serial) {
                DPRINTF("Inquiry (EVPD[Serial number] not supported\n");
                return -1;
            }

            l = strlen(s->serial);
            if (l > req->cmd.xfer)
                l = req->cmd.xfer;
            if (l > 20)
                l = 20;

            DPRINTF("Inquiry EVPD[Serial number] "
                    "buffer size %zd\n", req->cmd.xfer);
            outbuf[buflen++] = l;
            memcpy(outbuf+buflen, s->serial, l);
            buflen += l;
            break;
        }

        case 0x83:  
        {
            int max_len = 255 - 8;
            int id_len = strlen(bdrv_get_device_name(s->bs));

            if (id_len > max_len)
                id_len = max_len;
            DPRINTF("Inquiry EVPD[Device identification] "
                    "buffer size %zd\n", req->cmd.xfer);

            outbuf[buflen++] = 4 + id_len;
            outbuf[buflen++] = 0x2;  
            outbuf[buflen++] = 0;    
            outbuf[buflen++] = 0;    
            outbuf[buflen++] = id_len;  

            memcpy(outbuf+buflen, bdrv_get_device_name(s->bs), id_len);
            buflen += id_len;
            break;
        }
        case 0xb0:  
        {
            unsigned int unmap_sectors =
                    s->qdev.conf.discard_granularity / s->qdev.blocksize;
            unsigned int min_io_size =
                    s->qdev.conf.min_io_size / s->qdev.blocksize;
            unsigned int opt_io_size =
                    s->qdev.conf.opt_io_size / s->qdev.blocksize;

            if (s->qdev.type == TYPE_ROM) {
                DPRINTF("Inquiry (EVPD[%02X] not supported for CDROM\n",
                        page_code);
                return -1;
            }
             
            outbuf[3] = buflen = 0x3c;

            memset(outbuf + 4, 0, buflen - 4);

             
            outbuf[6] = (min_io_size >> 8) & 0xff;
            outbuf[7] = min_io_size & 0xff;

             
            outbuf[12] = (opt_io_size >> 24) & 0xff;
            outbuf[13] = (opt_io_size >> 16) & 0xff;
            outbuf[14] = (opt_io_size >> 8) & 0xff;
            outbuf[15] = opt_io_size & 0xff;

             
            outbuf[28] = (unmap_sectors >> 24) & 0xff;
            outbuf[29] = (unmap_sectors >> 16) & 0xff;
            outbuf[30] = (unmap_sectors >> 8) & 0xff;
            outbuf[31] = unmap_sectors & 0xff;
            break;
        }
        case 0xb2:  
        {
            outbuf[3] = buflen = 8;
            outbuf[4] = 0;
            outbuf[5] = 0x40;  
            outbuf[6] = 0;
            outbuf[7] = 0;
            break;
        }
        default:
            BADF("Error: unsupported Inquiry (EVPD[%02X]) "
                 "buffer size %zd\n", page_code, req->cmd.xfer);
            return -1;
        }
         
        return buflen;
    }

     
    if (req->cmd.buf[2] != 0) {
        BADF("Error: Inquiry (STANDARD) page or code "
             "is non-zero [%02X]\n", req->cmd.buf[2]);
        return -1;
    }

     
    if (req->cmd.xfer < 5) {
        BADF("Error: Inquiry (STANDARD) buffer size %zd "
             "is less than 5\n", req->cmd.xfer);
        return -1;
    }

    buflen = req->cmd.xfer;
    if (buflen > SCSI_MAX_INQUIRY_LEN)
        buflen = SCSI_MAX_INQUIRY_LEN;

    memset(outbuf, 0, buflen);

    outbuf[0] = s->qdev.type & 0x1f;
    if (s->qdev.type == TYPE_ROM) {
        outbuf[1] = 0x80;
        memcpy(&outbuf[16], "QEMU CD-ROM     ", 16);
    } else {
        outbuf[1] = s->removable ? 0x80 : 0;
        memcpy(&outbuf[16], "QEMU HARDDISK   ", 16);
    }
    memcpy(&outbuf[8], "QEMU    ", 8);
    memset(&outbuf[32], 0, 4);
    memcpy(&outbuf[32], s->version, MIN(4, strlen(s->version)));
     
    outbuf[2] = 5;
    outbuf[3] = 2;  

    if (buflen > 36) {
        outbuf[4] = buflen - 5;  
    } else {
         
        outbuf[4] = 36 - 5;
    }

     
    outbuf[7] = 0x10 | (req->bus->tcq ? 0x02 : 0);
    return buflen;
}
