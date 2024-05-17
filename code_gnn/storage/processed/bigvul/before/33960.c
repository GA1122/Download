send_snmp_trap(const char *node, const char *rsc, const char *task, int target_rc, int rc,
               int status, const char *desc)
{
    int ret = 1;

#if ENABLE_SNMP
    static oid snmptrap_oid[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };
    static oid sysuptime_oid[] = { 1, 3, 6, 1, 2, 1, 1, 3, 0 };

    netsnmp_pdu *trap_pdu;
    netsnmp_session *session = crm_snmp_init(snmp_target, snmp_community);

    trap_pdu = snmp_pdu_create(SNMP_MSG_TRAP2);
    if (!trap_pdu) {
        crm_err("Failed to create SNMP notification");
        return SNMPERR_GENERR;
    }

    if (1) {
         
        char csysuptime[20];
        time_t now = time(NULL);

        sprintf(csysuptime, "%ld", now);
        snmp_add_var(trap_pdu, sysuptime_oid, sizeof(sysuptime_oid) / sizeof(oid), 't', csysuptime);
    }

     
    ret =
        snmp_add_var(trap_pdu, snmptrap_oid, sizeof(snmptrap_oid) / sizeof(oid), 'o',
                     snmp_crm_trap_oid);
    if (ret != 0) {
        crm_err("Failed set snmpTrapOid.0=%s", snmp_crm_trap_oid);
        return ret;
    }

     
    if (rsc) {
        add_snmp_field(trap_pdu, snmp_crm_oid_rsc, rsc);
    }
    add_snmp_field(trap_pdu, snmp_crm_oid_node, node);
    add_snmp_field(trap_pdu, snmp_crm_oid_task, task);
    add_snmp_field(trap_pdu, snmp_crm_oid_desc, desc);

    add_snmp_field_int(trap_pdu, snmp_crm_oid_rc, rc);
    add_snmp_field_int(trap_pdu, snmp_crm_oid_trc, target_rc);
    add_snmp_field_int(trap_pdu, snmp_crm_oid_status, status);

     
    ret = snmp_send(session, trap_pdu);
    if (ret == 0) {
         
        snmp_sess_perror("Could not send SNMP trap", session);
        snmp_free_pdu(trap_pdu);
        ret = SNMPERR_GENERR;
    } else {
        ret = SNMPERR_SUCCESS;
    }
#else
    crm_err("Sending SNMP traps is not supported by this installation");
#endif
    return ret;
}