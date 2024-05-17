gs_pattern1_get_pattern(const gs_pattern_instance_t *pinst)
{
    return (const gs_pattern_template_t *)
        &((const gs_pattern1_instance_t *)pinst)->templat;
}
