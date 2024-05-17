static inline int btpan_role_to_bta(int btpan_role)
{
 int bta_pan_role = PAN_ROLE_INACTIVE;
    BTIF_TRACE_DEBUG("btpan_role:0x%x", btpan_role);
 if (btpan_role & BTPAN_ROLE_PANNAP)
        bta_pan_role |= PAN_ROLE_NAP_SERVER;
 if (btpan_role & BTPAN_ROLE_PANU)
        bta_pan_role |= PAN_ROLE_CLIENT;
 return bta_pan_role;
}
