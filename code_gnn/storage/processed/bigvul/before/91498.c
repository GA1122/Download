compress(int init_bits, gdIOCtxPtr outfile, gdImagePtr im, GifCtx *ctx)
{
    register long fcode;
    register code_int i  ;
    register int c;
    register code_int ent;
    register code_int disp;
    register code_int hsize_reg;
    register int hshift;

     
    ctx->g_init_bits = init_bits;
    ctx->g_outfile = outfile;

     
    ctx->offset = 0;
    ctx->out_count = 0;
    ctx->clear_flg = 0;
    ctx->in_count = 1;
    ctx->maxcode = MAXCODE(ctx->n_bits = ctx->g_init_bits);

    ctx->ClearCode = (1 << (init_bits - 1));
    ctx->EOFCode = ctx->ClearCode + 1;
    ctx->free_ent = ctx->ClearCode + 2;

    char_init(ctx);

    ent = GIFNextPixel( im, ctx );

    hshift = 0;
    for ( fcode = (long) hsize;  fcode < 65536L; fcode *= 2L )
        ++hshift;
    hshift = 8 - hshift;                 

    hsize_reg = hsize;
    cl_hash( (count_int) hsize_reg, ctx );             

    output( (code_int)ctx->ClearCode, ctx );

#ifdef SIGNED_COMPARE_SLOW
    while ( (c = GIFNextPixel( im )) != (unsigned) EOF ) {
#else  
    while ( (c = GIFNextPixel( im, ctx )) != EOF ) {   
#endif  

        ++(ctx->in_count);

        fcode = (long) (((long) c << maxbits) + ent);
        i = (((code_int)c << hshift) ^ ent);     

        if ( HashTabOf (i) == fcode ) {
            ent = CodeTabOf (i);
            continue;
        } else if ( (long)HashTabOf (i) < 0 )       
            goto nomatch;
        disp = hsize_reg - i;            
        if ( i == 0 )
            disp = 1;
probe:
        if ( (i -= disp) < 0 )
            i += hsize_reg;

        if ( HashTabOf (i) == fcode ) {
            ent = CodeTabOf (i);
            continue;
        }
        if ( (long)HashTabOf (i) > 0 )
            goto probe;
nomatch:
        output ( (code_int) ent, ctx );
        ++(ctx->out_count);
        ent = c;
#ifdef SIGNED_COMPARE_SLOW
        if ( (unsigned) ctx->free_ent < (unsigned) maxmaxcode) {
#else  
        if ( ctx->free_ent < maxmaxcode ) {   
#endif  
            CodeTabOf (i) = ctx->free_ent++;  
            HashTabOf (i) = fcode;
        } else
                cl_block(ctx);
    }
     
    output( (code_int)ent, ctx );
    ++(ctx->out_count);
    output( (code_int) ctx->EOFCode, ctx );
}
