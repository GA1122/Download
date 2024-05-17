gx_pattern1_get_transptr(const gx_device_color *pdevc)
{
    if (pdevc->colors.pattern.p_tile != NULL)
        return pdevc->colors.pattern.p_tile->ttrans;
    else
        return NULL;
}
