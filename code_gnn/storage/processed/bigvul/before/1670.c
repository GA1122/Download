gx_dc_devn_masked_equal(const gx_device_color * pdevc1,
                        const gx_device_color * pdevc2)
{
    return (*gx_dc_type_devn->equal) (pdevc1, pdevc2) &&
        pdevc1->mask.id == pdevc2->mask.id;
}
