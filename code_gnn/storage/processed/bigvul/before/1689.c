gx_pattern1_clist_has_trans(const gx_device_color *pdevc)
{
    if (pdevc->colors.pattern.p_tile != NULL &&
        pdevc->colors.pattern.p_tile->cdev != NULL) {
        return pdevc->colors.pattern.p_tile->cdev->common.page_uses_transparency;
    } else {
        return 0;
    }
}
