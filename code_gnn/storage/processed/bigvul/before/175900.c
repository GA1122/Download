static bool lmp_version_below(const RawAddress& bda, uint8_t version) {
  tACL_CONN* acl = btm_bda_to_acl(bda, BT_TRANSPORT_LE);
 if (acl == NULL || acl->lmp_version == 0) {
    SMP_TRACE_WARNING("%s cannot retrieve LMP version...", __func__);
 return false;
 }
  SMP_TRACE_WARNING("%s LMP version %d < %d", __func__, acl->lmp_version,
                    version);
 return acl->lmp_version < version;
}
