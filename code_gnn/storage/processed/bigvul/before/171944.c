static inline int bta_role_to_btpan(int bta_pan_role)
{
 int btpan_role = 0;
    BTIF_TRACE_DEBUG("bta_pan_role:0x%x", bta_pan_role);
 if (bta_pan_role & PAN_ROLE_NAP_SERVER)
        btpan_role |= BTPAN_ROLE_PANNAP;
 if (bta_pan_role & PAN_ROLE_CLIENT)
        btpan_role |= BTPAN_ROLE_PANU;
 return btpan_role;
}
