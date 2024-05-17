gsicc_init_gs_colors(gs_gstate *pgs)
{
    int             code = 0;
    gs_color_space  *cs_old;
    gs_color_space  *cs_new;
    int k;

    if (pgs->in_cachedevice)
        return_error(gs_error_undefined);

    for (k = 0; k < 2; k++) {
         
        cs_old = pgs->color[k].color_space;
        cs_new = gs_cspace_new_DeviceGray(pgs->memory);
        rc_increment_cs(cs_new);
        pgs->color[k].color_space = cs_new;
        if ( (code = cs_new->type->install_cspace(cs_new, pgs)) < 0 ) {
            pgs->color[k].color_space = cs_old;
            rc_decrement_only_cs(cs_new, "gsicc_init_gs_colors");
            return code;
        } else {
            rc_decrement_only_cs(cs_old, "gsicc_init_gs_colors");
        }
    }
    return code;
}
