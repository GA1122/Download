static int tls1_check_ec_key(SSL *s,
                             unsigned char *curve_id, unsigned char *comp_id)
{
    const unsigned char *pformats, *pcurves;
    size_t num_formats, num_curves, i;
    int j;
     
    if (comp_id && s->session->tlsext_ecpointformatlist) {
        pformats = s->session->tlsext_ecpointformatlist;
        num_formats = s->session->tlsext_ecpointformatlist_length;
        for (i = 0; i < num_formats; i++, pformats++) {
            if (*comp_id == *pformats)
                break;
        }
        if (i == num_formats)
            return 0;
    }
    if (!curve_id)
        return 1;
     
    for (j = 0; j <= 1; j++) {
        if (!tls1_get_curvelist(s, j, &pcurves, &num_curves))
            return 0;
        for (i = 0; i < num_curves; i++, pcurves += 2) {
            if (pcurves[0] == curve_id[0] && pcurves[1] == curve_id[1])
                break;
        }
        if (i == num_curves)
            return 0;
         
        if (!s->server)
            return 1;
    }
    return 1;
}
