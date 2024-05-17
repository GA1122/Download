handle_rsc_op(xmlNode * rsc_op)
{
    int rc = -1;
    int status = -1;
    int action = -1;
    int interval = 0;
    int target_rc = -1;
    int transition_num = -1;
    gboolean notify = TRUE;

    char *rsc = NULL;
    char *task = NULL;
    const char *desc = NULL;
    const char *node = NULL;
    const char *magic = NULL;
    const char *id = crm_element_value(rsc_op, XML_LRM_ATTR_TASK_KEY);
    char *update_te_uuid = NULL;

    xmlNode *n = rsc_op;

    if (id == NULL) {
         
        id = ID(rsc_op);
    }

    magic = crm_element_value(rsc_op, XML_ATTR_TRANSITION_MAGIC);
    if (magic == NULL) {
         
        return;
    }

    if (FALSE == decode_transition_magic(magic, &update_te_uuid, &transition_num, &action,
                                         &status, &rc, &target_rc)) {
        crm_err("Invalid event %s detected for %s", magic, id);
        return;
    }

    if (parse_op_key(id, &rsc, &task, &interval) == FALSE) {
        crm_err("Invalid event detected for %s", id);
        goto bail;
    }

    while (n != NULL && safe_str_neq(XML_CIB_TAG_STATE, TYPE(n))) {
        n = n->parent;
    }

    node = crm_element_value(n, XML_ATTR_UNAME);
    if (node == NULL) {
        node = ID(n);
    }
    if (node == NULL) {
        crm_err("No node detected for event %s (%s)", magic, id);
        goto bail;
    }

     
    desc = pcmk_strerror(pcmk_ok);
    if (status == PCMK_LRM_OP_DONE && target_rc == rc) {
        crm_notice("%s of %s on %s completed: %s", task, rsc, node, desc);
        if (rc == PCMK_EXECRA_NOT_RUNNING) {
            notify = FALSE;
        }

    } else if (status == PCMK_LRM_OP_DONE) {
        desc = lrmd_event_rc2str(rc);
        crm_warn("%s of %s on %s failed: %s", task, rsc, node, desc);

    } else {
        desc = services_lrm_status_str(status);
        crm_warn("%s of %s on %s failed: %s", task, rsc, node, desc);
    }

    if (notify && snmp_target) {
        send_snmp_trap(node, rsc, task, target_rc, rc, status, desc);
    }
    if (notify && crm_mail_to) {
        send_smtp_trap(node, rsc, task, target_rc, rc, status, desc);
    }
    if (notify && external_agent) {
        send_custom_trap(node, rsc, task, target_rc, rc, status, desc);
    }
  bail:
    free(update_te_uuid);
    free(rsc);
    free(task);
}