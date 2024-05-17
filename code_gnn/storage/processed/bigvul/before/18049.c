jbig2_table(Jbig2Ctx *ctx, Jbig2Segment *segment, const byte *segment_data)
{
    Jbig2HuffmanParams *params = NULL;
    Jbig2HuffmanLine *line = NULL;

    segment->result = NULL;
    if (segment->data_length < 10)
        goto too_short;

    {
         
        const int code_table_flags = segment_data[0];
        const int HTOOB = code_table_flags & 0x01;       

         
        const int HTPS = (code_table_flags >> 1 & 0x07) + 1;

         
        const int HTRS = (code_table_flags >> 4 & 0x07) + 1;

         
        const int32_t HTLOW = jbig2_get_int32(segment_data + 1);

         
        const int32_t HTHIGH = jbig2_get_int32(segment_data + 5);

         
        const size_t lines_max = (segment->data_length * 8 - HTPS * (HTOOB ? 3 : 2)) / (HTPS + HTRS) + (HTOOB ? 3 : 2);

         
        const byte *lines_data = segment_data + 9;
        const size_t lines_data_bitlen = (segment->data_length - 9) * 8;         

         
        size_t boffset = 0;

         
        int32_t CURRANGELOW = HTLOW;
        size_t NTEMP = 0;

#ifdef JBIG2_DEBUG
        jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, segment->number,
                    "DECODING USER TABLE... Flags: %d, HTOOB: %d, HTPS: %d, HTRS: %d, HTLOW: %d, HTHIGH: %d",
                    code_table_flags, HTOOB, HTPS, HTRS, HTLOW, HTHIGH);
#endif

         
        params = jbig2_new(ctx, Jbig2HuffmanParams, 1);
        if (params == NULL) {
            jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Could not allocate Huffman Table Parameter");
            goto error_exit;
        }
        line = jbig2_new(ctx, Jbig2HuffmanLine, lines_max);
        if (line == NULL) {
            jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Could not allocate Huffman Table Lines");
            goto error_exit;
        }
         
        while (CURRANGELOW < HTHIGH) {
             
            if (boffset + HTPS >= lines_data_bitlen)
                goto too_short;
            line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
             
            if (boffset + HTRS >= lines_data_bitlen)
                goto too_short;
            line[NTEMP].RANGELEN = jbig2_table_read_bits(lines_data, &boffset, HTRS);
             
            line[NTEMP].RANGELOW = CURRANGELOW;
            CURRANGELOW += (1 << line[NTEMP].RANGELEN);
            NTEMP++;
        }
         
        if (boffset + HTPS >= lines_data_bitlen)
            goto too_short;
        line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
        line[NTEMP].RANGELEN = 32;
        line[NTEMP].RANGELOW = HTLOW - 1;
        NTEMP++;
         
        if (boffset + HTPS >= lines_data_bitlen)
            goto too_short;
        line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
        line[NTEMP].RANGELEN = 32;
        line[NTEMP].RANGELOW = HTHIGH;
        NTEMP++;
         
        if (HTOOB) {
             
            if (boffset + HTPS >= lines_data_bitlen)
                goto too_short;
            line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
            line[NTEMP].RANGELEN = 0;
            line[NTEMP].RANGELOW = 0;
            NTEMP++;
        }
        if (NTEMP != lines_max) {
            Jbig2HuffmanLine *new_line = jbig2_renew(ctx, line,
                                         Jbig2HuffmanLine, NTEMP);

            if (new_line == NULL) {
                jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Could not reallocate Huffman Table Lines");
                goto error_exit;
            }
            line = new_line;
        }
        params->HTOOB = HTOOB;
        params->n_lines = NTEMP;
        params->lines = line;
        segment->result = params;

#ifdef JBIG2_DEBUG
        {
            int i;

            for (i = 0; i < NTEMP; i++) {
                jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, segment->number,
                            "Line: %d, PREFLEN: %d, RANGELEN: %d, RANGELOW: %d",
                            i, params->lines[i].PREFLEN, params->lines[i].RANGELEN, params->lines[i].RANGELOW);
            }
        }
#endif
    }
    return 0;

too_short:
    jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Segment too short");
error_exit:
    if (line != NULL) {
        jbig2_free(ctx->allocator, line);
    }
    if (params != NULL) {
        jbig2_free(ctx->allocator, params);
    }
    return -1;
}
