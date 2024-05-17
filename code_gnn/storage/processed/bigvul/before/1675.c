gx_dc_pattern_equal(const gx_device_color * pdevc1,
                    const gx_device_color * pdevc2)
{
    return pdevc2->type == pdevc1->type &&
        pdevc1->phase.x == pdevc2->phase.x &&
        pdevc1->phase.y == pdevc2->phase.y &&
        pdevc1->mask.id == pdevc2->mask.id;
}
