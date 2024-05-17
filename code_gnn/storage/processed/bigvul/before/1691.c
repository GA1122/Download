gx_pattern_cache_lookup(gx_device_color * pdevc, const gs_gstate * pgs,
                        gx_device * dev, gs_color_select_t select)
{
    gx_pattern_cache *pcache = pgs->pattern_cache;
    gx_bitmap_id id = pdevc->mask.id;

    if (id == gx_no_bitmap_id) {
        color_set_null_pattern(pdevc);
        return true;
    }
    if (pcache != 0) {
        gx_color_tile *ctile = &pcache->tiles[id % pcache->num_tiles];
        bool internal_accum = true;
        if (pgs->have_pattern_streams) {
            int code = dev_proc(dev, dev_spec_op)(dev, gxdso_pattern_load, NULL, id);
            internal_accum = (code == 0);
            if (code < 0)
                return false;
        }
        if (ctile->id == id &&
            ctile->is_dummy == !internal_accum
            ) {
            int px = pgs->screen_phase[select].x;
            int py = pgs->screen_phase[select].y;

            if (gx_dc_is_pattern1_color(pdevc)) {        
                pdevc->colors.pattern.p_tile = ctile;
#           if 0  
                color_set_phase_mod(pdevc, px, py,
                                    ctile->tbits.rep_width,
                                    ctile->tbits.rep_height);
#               else
                color_set_phase(pdevc, -px, -py);
#               endif
            }
            pdevc->mask.m_tile =
                (ctile->tmask.data == 0 ? (gx_color_tile *) 0 :
                 ctile);
            pdevc->mask.m_phase.x = -px;
            pdevc->mask.m_phase.y = -py;
            return true;
        }
    }
    return false;
}
