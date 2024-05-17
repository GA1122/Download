static int search_chunk(struct mschmd_header *chm,
                        const unsigned char *chunk,
                        const char *filename,
                        const unsigned char **result,
                        const unsigned char **result_end)
{
    const unsigned char *start, *end, *p;
    unsigned int qr_size, num_entries, qr_entries, qr_density, name_len;
    unsigned int L, R, M, fname_len, entries_off, is_pmgl;
    int cmp;

    fname_len = strlen(filename);

     
    if (chunk[3] == 0x4C) {
        is_pmgl = 1;
        entries_off = pmgl_Entries;
    }
    else {
        is_pmgl = 0;
        entries_off = pmgi_Entries;
    }

     
    qr_size     = EndGetI32(&chunk[pmgl_QuickRefSize]);
    start       = &chunk[chm->chunk_size - 2];
    end         = &chunk[chm->chunk_size - qr_size];
    num_entries = EndGetI16(start);
    qr_density  = 1 + (1 << chm->density);
    qr_entries  = (num_entries + qr_density-1) / qr_density;

    if (num_entries == 0) {
        D(("chunk has no entries"))
        return -1;
    }

    if (qr_size > chm->chunk_size) {
        D(("quickref size > chunk size"))
        return -1;
    }

    *result_end = end;

    if (((int)qr_entries * 2) > (start - end)) {
        D(("WARNING; more quickrefs than quickref space"))
        qr_entries = 0;  
    }

    if (qr_entries > 0) {
        L = 0;
        R = qr_entries - 1;
        do {
             
            M = (L + R) >> 1;

             
            p = &chunk[entries_off + (M ? EndGetI16(start - (M << 1)) : 0)];
            READ_ENCINT(name_len);
            if (name_len > (unsigned int) (end - p)) goto chunk_end;
            cmp = compare(filename, (char *)p, fname_len, name_len);

            if (cmp == 0) break;
            else if (cmp < 0) { if (M) R = M - 1; else return 0; }
            else if (cmp > 0) L = M + 1;
        } while (L <= R);
        M = (L + R) >> 1;

        if (cmp == 0) {
             
            p += name_len;
            *result = p;
            return 1;
        }

         
        p = &chunk[entries_off + (M ? EndGetI16(start - (M << 1)) : 0)];
        num_entries -= (M * qr_density);
        if (num_entries > qr_density) num_entries = qr_density;
    }
    else {
        p = &chunk[entries_off];
    }

     
    *result = NULL;
    while (num_entries-- > 0) {
        READ_ENCINT(name_len);
        if (name_len > (unsigned int) (end - p)) goto chunk_end;
        cmp = compare(filename, (char *)p, fname_len, name_len);
        p += name_len;

        if (cmp == 0) {
             
            *result = p;
            return 1;
        }

        if (cmp < 0) {
             
            break;
        }

         
        if (is_pmgl) {
            READ_ENCINT(R);  
            READ_ENCINT(R);  
            READ_ENCINT(R);  
        }
        else {
            *result = p;  
            READ_ENCINT(R);  
        }
    }

      
     return (is_pmgl) ? 0 : (*result ? 1 : 0);

 chunk_end:
    D(("reached end of chunk data while searching"))
    return -1;
}
