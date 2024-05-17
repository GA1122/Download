do_compress( compress_filter_context_t *zfx, z_stream *zs, int flush, IOBUF a )
{
    int zrc;
    unsigned n;

    do {
#ifndef __riscos__
	zs->next_out = zfx->outbuf;
#else  
	zs->next_out = (Bytef *) zfx->outbuf;
#endif  
	zs->avail_out = zfx->outbufsize;
	if( DBG_FILTER )
	    log_debug("enter deflate: avail_in=%u, avail_out=%u, flush=%d\n",
		    (unsigned)zs->avail_in, (unsigned)zs->avail_out, flush );
	zrc = deflate( zs, flush );
	if( zrc == Z_STREAM_END && flush == Z_FINISH )
	    ;
	else if( zrc != Z_OK ) {
	    if( zs->msg )
		log_fatal("zlib deflate problem: %s\n", zs->msg );
	    else
		log_fatal("zlib deflate problem: rc=%d\n", zrc );
	}
	n = zfx->outbufsize - zs->avail_out;
	if( DBG_FILTER )
	    log_debug("leave deflate: "
		      "avail_in=%u, avail_out=%u, n=%u, zrc=%d\n",
		(unsigned)zs->avail_in, (unsigned)zs->avail_out,
					       (unsigned)n, zrc );

	if( iobuf_write( a, zfx->outbuf, n ) ) {
	    log_debug("deflate: iobuf_write failed\n");
	    return G10ERR_WRITE_FILE;
	}
    } while( zs->avail_in || (flush == Z_FINISH && zrc != Z_STREAM_END) );
    return 0;
}
