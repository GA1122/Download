local void infchk(void)
{
    int ret, cont, was;
    unsigned long check, len;
    z_stream strm;
    unsigned tmp2;
    unsigned long tmp4;
    off_t clen;

    cont = 0;
    do {
         
        g.in_tot = g.in_left;        
        g.out_tot = 0;
        g.out_check = CHECK(0L, Z_NULL, 0);
        strm.zalloc = ZALLOC;
        strm.zfree = ZFREE;
        strm.opaque = OPAQUE;
        ret = inflateBackInit(&strm, 15, out_buf);
        if (ret != Z_OK)
            bail("not enough memory", "");

         
        strm.avail_in = g.in_left;
        strm.next_in = g.in_next;
        ret = inflateBack(&strm, inb, NULL, outb, NULL);
        if (ret != Z_STREAM_END)
            bail("corrupted input -- invalid deflate data: ", g.inf);
        g.in_left = strm.avail_in;
        g.in_next = strm.next_in;
        inflateBackEnd(&strm);
        outb(NULL, NULL, 0);         

         
        clen = g.in_tot - g.in_left;

         
        if (g.form > 1) {            
            if (g.form == 3) {       
                 
                g.zip_crc = GET4();
                g.zip_clen = GET4();
                g.zip_ulen = GET4();
                if (g.in_eof)
                    bail("corrupted zip entry -- missing trailer: ", g.inf);

                 
                if (g.zip_crc != g.out_check) {
                    if (g.zip_crc != 0x08074b50UL || g.zip_clen != g.out_check)
                        bail("corrupted zip entry -- crc32 mismatch: ", g.inf);
                    g.zip_crc = g.zip_clen;
                    g.zip_clen = g.zip_ulen;
                    g.zip_ulen = GET4();
                }

                 
                else if (g.zip_crc == 0x08074b50UL &&
                         g.zip_clen == g.zip_crc &&
                         ((clen & LOW32) != g.zip_crc ||
                          g.zip_ulen == g.zip_crc)) {
                    g.zip_crc = g.zip_clen;
                    g.zip_clen = g.zip_ulen;
                    g.zip_ulen = GET4();
                }

                 
                if (g.zip_ulen != (g.out_tot & LOW32)) {
                    g.zip_ulen = GET4();
                    (void)GET4();
                }
                if (g.in_eof)
                    bail("corrupted zip entry -- missing trailer: ", g.inf);
            }
            if (g.zip_clen != (clen & LOW32) ||
                g.zip_ulen != (g.out_tot & LOW32))
                bail("corrupted zip entry -- length mismatch: ", g.inf);
            check = g.zip_crc;
        }
        else if (g.form == 1) {      
            check = (unsigned long)(GET()) << 24;
            check += (unsigned long)(GET()) << 16;
            check += (unsigned)(GET()) << 8;
            check += GET();
            if (g.in_eof)
                bail("corrupted zlib stream -- missing trailer: ", g.inf);
            if (check != g.out_check)
                bail("corrupted zlib stream -- adler32 mismatch: ", g.inf);
        }
        else {                       
            check = GET4();
            len = GET4();
            if (g.in_eof)
                bail("corrupted gzip stream -- missing trailer: ", g.inf);
            if (check != g.out_check)
                bail("corrupted gzip stream -- crc32 mismatch: ", g.inf);
            if (len != (g.out_tot & LOW32))
                bail("corrupted gzip stream -- length mismatch: ", g.inf);
        }

         
        if (g.list) {
            g.in_tot = clen;
            show_info(8, check, g.out_tot, cont);
            cont = 1;
        }

         
        was = g.form;
    } while (was == 0 && (ret = get_header(0)) == 8 && g.form == 0);

     
    if (was == 0 && ret == -2 && g.force && g.pipeout && g.decode != 2 &&
        !g.list)
        cat();
    else if (was > 1 && get_header(0) != -5)
        complain("entries after the first in %s were ignored", g.inf);
    else if ((was == 0 && ret != -1) || (was == 1 && (GET(), !g.in_eof)))
        complain("%s OK, has trailing junk which was ignored", g.inf);
}