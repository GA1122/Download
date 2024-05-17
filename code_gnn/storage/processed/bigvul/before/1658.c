gs_makepattern(gs_client_color * pcc, const gs_pattern1_template_t * pcp,
               const gs_matrix * pmat, gs_gstate * pgs, gs_memory_t * mem)
{
    return gs_pattern1_make_pattern(pcc, (const gs_pattern_template_t *)pcp,
                                    pmat, pgs, mem);
}
