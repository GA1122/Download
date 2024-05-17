static void load_creator(FILE *fp, pdf_t *pdf)
{
    int    i, buf_idx;
    char   c, *buf, obj_id_buf[32] = {0};
    long   start;
    size_t sz;

    start = ftell(fp);

     
    for (i=0; i<pdf->n_xrefs; ++i)
    {
        if (!pdf->xrefs[i].version)
          continue;

         
        fseek(fp, pdf->xrefs[i].start, SEEK_SET);
        while (SAFE_F(fp, (fgetc(fp) != 't')))
            ;  

         
        c = '\0';
        while (SAFE_F(fp, ((c = fgetc(fp)) != '>')))
          if (SAFE_F(fp, ((c == '/') &&
                          (fgetc(fp) == 'I') && ((fgetc(fp) == 'n')))))
            break;

         
        END_OF_TRAILER(c, start, fp);

        while (SAFE_F(fp, (!isspace(c = fgetc(fp)) && (c != '>'))))
            ;  

         
        END_OF_TRAILER(c, start, fp);

        while (SAFE_F(fp, (isspace(c = fgetc(fp)) && (c != '>'))))
            ;  

         
        END_OF_TRAILER(c, start, fp);

         
        buf_idx = 0;
        obj_id_buf[buf_idx++] = c;
        while ((buf_idx < (sizeof(obj_id_buf) - 1)) &&
               SAFE_F(fp, (!isspace(c = fgetc(fp)) && (c != '>'))))
          obj_id_buf[buf_idx++] = c;

        END_OF_TRAILER(c, start, fp);
     
          
        buf = get_object(fp, atoll(obj_id_buf), &pdf->xrefs[i], &sz, NULL);
        if (!buf && pdf->xrefs[i].is_linear && (i+1 < pdf->n_xrefs))
          buf = get_object(fp, atoll(obj_id_buf), &pdf->xrefs[i+1], &sz, NULL);

        load_creator_from_buf(fp, &pdf->xrefs[i], buf);
        free(buf);
    }

    fseek(fp, start, SEEK_SET);
}
