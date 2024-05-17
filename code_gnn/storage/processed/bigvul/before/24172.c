ar6000_dbglog_get_debug_logs(struct ar6_softc *ar)
{
    u32 data[8];  
    u32 address;
    u32 length;
    u32 dropped;
    u32 firstbuf;
    u32 debug_hdr_ptr;

    if (!ar->dbglog_init_done) return A_ERROR;


    AR6000_SPIN_LOCK(&ar->arLock, 0);

    if (ar->dbgLogFetchInProgress) {
        AR6000_SPIN_UNLOCK(&ar->arLock, 0);
        return A_EBUSY;
    }

         
    ar->dbgLogFetchInProgress = true;

    AR6000_SPIN_UNLOCK(&ar->arLock, 0);

    debug_hdr_ptr = dbglog_get_debug_hdr_ptr(ar);
    printk("debug_hdr_ptr: 0x%x\n", debug_hdr_ptr);

     
    if (debug_hdr_ptr) {
        address = TARG_VTOP(ar->arTargetType, debug_hdr_ptr);
        length = 4   + 4  ;
        A_MEMZERO(data, sizeof(data));
        ar6000_ReadDataDiag(ar->arHifDevice, address, (u8 *)data, length);
        address = TARG_VTOP(ar->arTargetType, data[0]  );
        firstbuf = address;
        dropped = data[1];  
        length = 4   + 4   + 4   + 4   + 4   + 4  ;
        A_MEMZERO(data, sizeof(data));
        ar6000_ReadDataDiag(ar->arHifDevice, address, (u8 *)&data, length);

        do {
            address = TARG_VTOP(ar->arTargetType, data[1]  );
            length = data[3];  
            if ((length) && (length <= data[2]  )) {
                 
                if (ar->log_cnt > (DBGLOG_HOST_LOG_BUFFER_SIZE - length)) {
                    ar->log_cnt = 0;
                }
                if(0 != ar6000_ReadDataDiag(ar->arHifDevice, address,
                                    (u8 *)&ar->log_buffer[ar->log_cnt], length))
                {
                    break;
                }
                ar6000_dbglog_event(ar, dropped, (s8 *)&ar->log_buffer[ar->log_cnt], length);
                ar->log_cnt += length;
            } else {
                AR_DEBUG_PRINTF(ATH_DEBUG_DBG_LOG,("Length: %d (Total size: %d)\n",
                                data[3], data[2]));
            }

            address = TARG_VTOP(ar->arTargetType, data[0]  );
            length = 4   + 4   + 4   + 4   + 4   + 4  ;
            A_MEMZERO(data, sizeof(data));
            if(0 != ar6000_ReadDataDiag(ar->arHifDevice, address,
                                (u8 *)&data, length))
            {
                break;
            }

        } while (address != firstbuf);
    }

    ar->dbgLogFetchInProgress = false;

    return 0;
}
