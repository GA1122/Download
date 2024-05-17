compress_filter( void *opaque, int control,
		 IOBUF a, byte *buf, size_t *ret_len)
{
    size_t size = *ret_len;
    compress_filter_context_t *zfx = opaque;
    z_stream *zs = zfx->opaque;
    int rc=0;

    if( control == IOBUFCTRL_UNDERFLOW ) {
	if( !zfx->status ) {
	    zs = zfx->opaque = xmalloc_clear( sizeof *zs );
	    init_uncompress( zfx, zs );
	    zfx->status = 1;
	}

	zs->next_out = BYTEF_CAST (buf);
	zs->avail_out = size;
	zfx->outbufsize = size;  
	rc = do_uncompress( zfx, zs, a, ret_len );
    }
    else if( control == IOBUFCTRL_FLUSH ) {
	if( !zfx->status ) {
	    PACKET pkt;
	    PKT_compressed cd;
	    if(zfx->algo != COMPRESS_ALGO_ZIP
	       && zfx->algo != COMPRESS_ALGO_ZLIB)
	      BUG();
	    memset( &cd, 0, sizeof cd );
	    cd.len = 0;
	    cd.algorithm = zfx->algo;
	    init_packet( &pkt );
	    pkt.pkttype = PKT_COMPRESSED;
	    pkt.pkt.compressed = &cd;
	    if( build_packet( a, &pkt ))
		log_bug("build_packet(PKT_COMPRESSED) failed\n");
	    zs = zfx->opaque = xmalloc_clear( sizeof *zs );
	    init_compress( zfx, zs );
	    zfx->status = 2;
	}

	zs->next_in = BYTEF_CAST (buf);
	zs->avail_in = size;
	rc = do_compress( zfx, zs, Z_NO_FLUSH, a );
    }
    else if( control == IOBUFCTRL_FREE ) {
	if( zfx->status == 1 ) {
	    inflateEnd(zs);
	    xfree(zs);
	    zfx->opaque = NULL;
	    xfree(zfx->outbuf); zfx->outbuf = NULL;
	}
	else if( zfx->status == 2 ) {
	    zs->next_in = BYTEF_CAST (buf);
	    zs->avail_in = 0;
	    do_compress( zfx, zs, Z_FINISH, a );
	    deflateEnd(zs);
	    xfree(zs);
	    zfx->opaque = NULL;
	    xfree(zfx->outbuf); zfx->outbuf = NULL;
	}
        if (zfx->release)
          zfx->release (zfx);
    }
    else if( control == IOBUFCTRL_DESC )
	*(char**)buf = "compress_filter";
    return rc;
}
