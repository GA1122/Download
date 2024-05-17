import (ctrl_t ctrl, IOBUF inp, const char* fname,struct stats_s *stats,
	unsigned char **fpr,size_t *fpr_len,unsigned int options )
{
    PACKET *pending_pkt = NULL;
    KBNODE keyblock = NULL;   
    int rc = 0;

    getkey_disable_caches();

    if( !opt.no_armor ) {  
	armor_filter_context_t *afx;

        afx = new_armor_context ();
	afx->only_keyblocks = 1;
	push_armor_filter (afx, inp);
        release_armor_context (afx);
    }

    while( !(rc = read_block( inp, &pending_pkt, &keyblock) )) {
	if( keyblock->pkt->pkttype == PKT_PUBLIC_KEY )
          rc = import_one (ctrl, fname, keyblock,
                           stats, fpr, fpr_len, options, 0);
	else if( keyblock->pkt->pkttype == PKT_SECRET_KEY )
          rc = import_secret_one (ctrl, fname, keyblock, stats, options);
	else if( keyblock->pkt->pkttype == PKT_SIGNATURE
		 && keyblock->pkt->pkt.signature->sig_class == 0x20 )
	    rc = import_revoke_cert( fname, keyblock, stats );
	else {
	    log_info( _("skipping block of type %d\n"),
					    keyblock->pkt->pkttype );
	}
	release_kbnode(keyblock);
         
	if( rc )
	    break;
	if( !(++stats->count % 100) && !opt.quiet )
	    log_info(_("%lu keys processed so far\n"), stats->count );
    }
    if( rc == -1 )
	rc = 0;
    else if( rc && rc != G10ERR_INV_KEYRING )
	log_error( _("error reading '%s': %s\n"), fname, g10_errstr(rc));

    return rc;
}
