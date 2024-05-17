gs_getpattern(const gs_client_color * pcc)
{
    const gs_pattern_instance_t *pinst = pcc->pattern;

    return (pinst == 0 || pinst->type != &gs_pattern1_type ? 0 :
            &((const gs_pattern1_instance_t *)pinst)->templat);
}
