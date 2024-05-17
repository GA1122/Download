EXPORTED int mboxlist_insertremote(mbentry_t *mbentry,
                          struct txn **txn)
{
    int r = 0;

    if (mbentry->server) {
         
        if (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_UNIFIED &&
            !strcasecmp(mbentry->server, config_servername)) {
             
            mbentry->mbtype &= ~MBTYPE_REMOTE;
            mbentry->server = NULL;
        }
        else {
             
            mbentry->mbtype |= MBTYPE_REMOTE;
        }
    }

     
    r = mboxlist_update_entry(mbentry->name, mbentry, txn);

    switch (r) {
    case CYRUSDB_OK:
        break;
    case CYRUSDB_AGAIN:
        abort();  
        break;
    default:
        syslog(LOG_ERR, "DBERROR: error updating database %s: %s",
               mbentry->name, cyrusdb_strerror(r));
        r = IMAP_IOERROR;
        break;
    }

    return r;
}
