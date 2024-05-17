cib_process_command(xmlNode * request, xmlNode ** reply, xmlNode ** cib_diff, gboolean privileged)
{
    xmlNode *input = NULL;
    xmlNode *output = NULL;
    xmlNode *result_cib = NULL;
    xmlNode *current_cib = NULL;

#if ENABLE_ACL
    xmlNode *filtered_current_cib = NULL;
#endif

    int call_type = 0;
    int call_options = 0;
    int log_level = LOG_DEBUG_4;

    const char *op = NULL;
    const char *section = NULL;

    int rc = pcmk_ok;
    int rc2 = pcmk_ok;

    gboolean send_r_notify = FALSE;
    gboolean global_update = FALSE;
    gboolean config_changed = FALSE;
    gboolean manage_counters = TRUE;

    CRM_ASSERT(cib_status == pcmk_ok);

    *reply = NULL;
    *cib_diff = NULL;
    current_cib = the_cib;

     
    op = crm_element_value(request, F_CIB_OPERATION);
    crm_element_value_int(request, F_CIB_CALLOPTS, &call_options);
    rc = cib_get_operation_id(op, &call_type);

    if (rc == pcmk_ok && privileged == FALSE) {
        rc = cib_op_can_run(call_type, call_options, privileged, global_update);
    }

    rc2 = cib_op_prepare(call_type, request, &input, &section);
    if (rc == pcmk_ok) {
        rc = rc2;
    }

    if (rc != pcmk_ok) {
        crm_trace("Call setup failed: %s", pcmk_strerror(rc));
        goto done;

    } else if (cib_op_modifies(call_type) == FALSE) {
#if ENABLE_ACL
        if (acl_enabled(config_hash) == FALSE
            || acl_filter_cib(request, current_cib, current_cib, &filtered_current_cib) == FALSE) {
            rc = cib_perform_op(op, call_options, cib_op_func(call_type), TRUE,
                                section, request, input, FALSE, &config_changed,
                                current_cib, &result_cib, NULL, &output);

        } else if (filtered_current_cib == NULL) {
            crm_debug("Pre-filtered the entire cib");
            rc = -EACCES;

        } else {
            crm_debug("Pre-filtered the queried cib according to the ACLs");
            rc = cib_perform_op(op, call_options, cib_op_func(call_type), TRUE,
                                section, request, input, FALSE, &config_changed,
                                filtered_current_cib, &result_cib, NULL, &output);
        }
#else
        rc = cib_perform_op(op, call_options, cib_op_func(call_type), TRUE,
                            section, request, input, FALSE, &config_changed,
                            current_cib, &result_cib, NULL, &output);

#endif

        CRM_CHECK(result_cib == NULL, free_xml(result_cib));
        goto done;
    }

     
    global_update = crm_is_true(crm_element_value(request, F_CIB_GLOBAL_UPDATE));
    if (global_update) {
        manage_counters = FALSE;
        call_options |= cib_force_diff;

        CRM_CHECK(call_type == 3 || call_type == 4, crm_err("Call type: %d", call_type);
                  crm_log_xml_err(request, "bad op"));
    }
#ifdef SUPPORT_PRENOTIFY
    if ((call_options & cib_inhibit_notify) == 0) {
        cib_pre_notify(call_options, op, the_cib, input);
    }
#endif

    if (rc == pcmk_ok) {
        if (call_options & cib_inhibit_bcast) {
             
            crm_trace("Skipping update: inhibit broadcast");
            manage_counters = FALSE;
        }

        rc = cib_perform_op(op, call_options, cib_op_func(call_type), FALSE,
                            section, request, input, manage_counters, &config_changed,
                            current_cib, &result_cib, cib_diff, &output);

#if ENABLE_ACL
        if (acl_enabled(config_hash) == TRUE
            && acl_check_diff(request, current_cib, result_cib, *cib_diff) == FALSE) {
            rc = -EACCES;
        }
#endif

        if (rc == pcmk_ok && config_changed) {
            time_t now;
            char *now_str = NULL;
            const char *validation = crm_element_value(result_cib, XML_ATTR_VALIDATION);

            if (validation) {
                int current_version = get_schema_version(validation);
                int support_version = get_schema_version("pacemaker-1.1");

                 
                if (current_version >= support_version) {
                    const char *origin = crm_element_value(request, F_ORIG);

                    crm_xml_replace(result_cib, XML_ATTR_UPDATE_ORIG,
                                    origin ? origin : cib_our_uname);
                    crm_xml_replace(result_cib, XML_ATTR_UPDATE_CLIENT,
                                    crm_element_value(request, F_CIB_CLIENTNAME));
#if ENABLE_ACL
                    crm_xml_replace(result_cib, XML_ATTR_UPDATE_USER,
                                    crm_element_value(request, F_CIB_USER));
#endif
                }
            }

            now = time(NULL);
            now_str = ctime(&now);
            now_str[24] = EOS;   
            crm_xml_replace(result_cib, XML_CIB_ATTR_WRITTEN, now_str);
        }

        if (manage_counters == FALSE) {
            config_changed = cib_config_changed(current_cib, result_cib, cib_diff);
        }

         
        if (config_changed == FALSE && crm_str_eq(CIB_OP_REPLACE, op, TRUE)) {
            config_changed = TRUE;
        }
    }

    cib_add_digest(result_cib, *cib_diff);

    if (rc == pcmk_ok && (call_options & cib_dryrun) == 0) {
        rc = activateCibXml(result_cib, config_changed, op);
        if (rc == pcmk_ok && cib_internal_config_changed(*cib_diff)) {
            cib_read_config(config_hash, result_cib);
        }

        if (crm_str_eq(CIB_OP_REPLACE, op, TRUE)) {
            if (section == NULL) {
                send_r_notify = TRUE;

            } else if (safe_str_eq(section, XML_TAG_CIB)) {
                send_r_notify = TRUE;

            } else if (safe_str_eq(section, XML_CIB_TAG_NODES)) {
                send_r_notify = TRUE;

            } else if (safe_str_eq(section, XML_CIB_TAG_STATUS)) {
                send_r_notify = TRUE;
            }

        } else if (crm_str_eq(CIB_OP_ERASE, op, TRUE)) {
            send_r_notify = TRUE;
        }

    } else if (rc == -pcmk_err_dtd_validation) {
        if (output != NULL) {
            crm_log_xml_info(output, "cib:output");
            free_xml(output);
        }
#if ENABLE_ACL
        {
            xmlNode *filtered_result_cib = NULL;

            if (acl_enabled(config_hash) == FALSE
                || acl_filter_cib(request, current_cib, result_cib,
                                  &filtered_result_cib) == FALSE) {
                output = result_cib;

            } else {
                crm_debug("Filtered the result cib for output according to the ACLs");
                output = filtered_result_cib;
                if (result_cib != NULL) {
                    free_xml(result_cib);
                }
            }
        }
#else
        output = result_cib;
#endif

    } else {
        free_xml(result_cib);
    }

    if ((call_options & cib_inhibit_notify) == 0) {
        const char *call_id = crm_element_value(request, F_CIB_CALLID);
        const char *client = crm_element_value(request, F_CIB_CLIENTNAME);

#ifdef SUPPORT_POSTNOTIFY
        cib_post_notify(call_options, op, input, rc, the_cib);
#endif
        cib_diff_notify(call_options, client, call_id, op, input, rc, *cib_diff);
    }

    if (send_r_notify) {
        const char *origin = crm_element_value(request, F_ORIG);

        cib_replace_notify(origin, the_cib, rc, *cib_diff);
    }

    if (rc != pcmk_ok) {
        log_level = LOG_DEBUG_4;
        if (rc == -pcmk_err_dtd_validation && global_update) {
            log_level = LOG_WARNING;
            crm_log_xml_info(input, "cib:global_update");
        }

    } else if (config_changed) {
        log_level = LOG_DEBUG_3;
        if (cib_is_master) {
            log_level = LOG_NOTICE;
        }

    } else if (cib_is_master) {
        log_level = LOG_DEBUG_2;
    }

    log_cib_diff(log_level, *cib_diff, "cib:diff");

  done:
    if ((call_options & cib_discard_reply) == 0) {
        *reply = cib_construct_reply(request, output, rc);
        crm_log_xml_trace(*reply, "cib:reply");
    }
#if ENABLE_ACL
    if (filtered_current_cib != NULL) {
        free_xml(filtered_current_cib);
    }
#endif

    if (call_type >= 0) {
        cib_op_cleanup(call_type, call_options, &input, &output);
    }
    return rc;
}