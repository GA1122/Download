crm_diff_update(const char *event, xmlNode * msg)
{
    int rc = -1;
    long now = time(NULL);
    const char *op = NULL;

    print_dot();

    if (current_cib != NULL) {
        xmlNode *cib_last = current_cib;
        current_cib = NULL;

        rc = cib_apply_patch_event(msg, cib_last, &current_cib, LOG_DEBUG);
        free_xml(cib_last);

        switch(rc) {
            case pcmk_err_diff_resync:
            case pcmk_err_diff_failed:
                crm_warn("[%s] %s Patch aborted: %s (%d)", event, op, pcmk_strerror(rc), rc);
            case pcmk_ok:
                break;
            default:
                crm_warn("[%s] %s ABORTED: %s (%d)", event, op, pcmk_strerror(rc), rc);
                return;
        }
    }

    if (current_cib == NULL) {
        current_cib = get_cib_copy(cib);
    }

    if (crm_mail_to || snmp_target || external_agent) {
         
        xmlXPathObject *xpathObj =
            xpath_search(msg,
                         "//" F_CIB_UPDATE_RESULT "//" XML_TAG_DIFF_ADDED "//" XML_LRM_TAG_RSC_OP);
        if (xpathObj && xpathObj->nodesetval->nodeNr > 0) {
            int lpc = 0, max = xpathObj->nodesetval->nodeNr;

            for (lpc = 0; lpc < max; lpc++) {
                xmlNode *rsc_op = getXpathResult(xpathObj, lpc);

                handle_rsc_op(rsc_op);
            }
        }
        if (xpathObj) {
            xmlXPathFreeObject(xpathObj);
        }
    }

    if ((now - last_refresh) > (reconnect_msec / 1000)) {
         
        mon_refresh_display(NULL);

    } else {
        mainloop_set_trigger(refresh_trigger);
    }
}