print_xml_status(pe_working_set_t * data_set)
{
    FILE *stream = stdout;
    GListPtr gIter = NULL;
    node_t *dc = NULL;
    xmlNode *stack = NULL;
    xmlNode *quorum_node = NULL;
    const char *quorum_votes = "unknown";

    dc = data_set->dc_node;


    fprintf(stream, "<?xml version=\"1.0\"?>\n");
    fprintf(stream, "<crm_mon version=\"%s\">\n", VERSION);

     
    fprintf(stream, "    <summary>\n");

    if (print_last_updated) {
        time_t now = time(NULL);
        char *now_str = ctime(&now);

        now_str[24] = EOS;       
        fprintf(stream, "        <last_update time=\"%s\" />\n", now_str);
    }

    if (print_last_change) {
        const char *last_written = crm_element_value(data_set->input, XML_CIB_ATTR_WRITTEN);
        const char *user = crm_element_value(data_set->input, XML_ATTR_UPDATE_USER);
        const char *client = crm_element_value(data_set->input, XML_ATTR_UPDATE_CLIENT);
        const char *origin = crm_element_value(data_set->input, XML_ATTR_UPDATE_ORIG);

        fprintf(stream, "        <last_change time=\"%s\" user=\"%s\" client=\"%s\" origin=\"%s\" />\n",
            last_written ? last_written : "",
            user ? user : "",
            client ? client : "",
            origin ? origin : "");
    }

    stack = get_xpath_object("//nvpair[@name='cluster-infrastructure']",
        data_set->input,
        LOG_DEBUG);
    if (stack) {
        fprintf(stream, "        <stack type=\"%s\" />\n", crm_element_value(stack, XML_NVPAIR_ATTR_VALUE));
    }

    if (!dc) {
        fprintf(stream, "        <current_dc present=\"false\" />\n");
    } else {
        const char *quorum = crm_element_value(data_set->input, XML_ATTR_HAVE_QUORUM);
        const char *uname = dc->details->uname;
        const char *id = dc->details->id;
        xmlNode *dc_version = get_xpath_object("//nvpair[@name='dc-version']",
            data_set->input,
            LOG_DEBUG);
        fprintf(stream, "        <current_dc present=\"true\" version=\"%s\" name=\"%s\" id=\"%s\" with_quorum=\"%s\" />\n",
            dc_version ? crm_element_value(dc_version, XML_NVPAIR_ATTR_VALUE) : "",
            uname,
            id,
            quorum ? (crm_is_true(quorum) ? "true" : "false") : "false");
    }

    quorum_node = get_xpath_object("//nvpair[@name='" XML_ATTR_EXPECTED_VOTES "']",
                    data_set->input,
                    LOG_DEBUG);
    if (quorum_node) {
        quorum_votes = crm_element_value(quorum_node, XML_NVPAIR_ATTR_VALUE);
    }
    fprintf(stream, "        <nodes_configured number=\"%d\" expected_votes=\"%s\" />\n",
        g_list_length(data_set->nodes),
        quorum_votes);

    fprintf(stream, "        <resources_configured number=\"%d\" />\n", count_resources(data_set, NULL));

    fprintf(stream, "    </summary>\n");

     
    fprintf(stream, "    <nodes>\n");
    for (gIter = data_set->nodes; gIter != NULL; gIter = gIter->next) {
        node_t *node = (node_t *) gIter->data;
        const char *node_type = "unknown";

        switch (node->details->type) {
        case node_member:
            node_type = "member";
            break;
        case node_ping:
            node_type = "ping";
            break;
        }

        fprintf(stream, "        <node name=\"%s\" ", node->details->uname);
        fprintf(stream, "id=\"%s\" ", node->details->id);
        fprintf(stream, "online=\"%s\" ", node->details->online ? "true" : "false");
        fprintf(stream, "standby=\"%s\" ", node->details->standby ? "true" : "false");
        fprintf(stream, "standby_onfail=\"%s\" ", node->details->standby_onfail ? "true" : "false");
        fprintf(stream, "pending=\"%s\" ", node->details->pending ? "true" : "false");
        fprintf(stream, "unclean=\"%s\" ", node->details->unclean ? "true" : "false");
        fprintf(stream, "shutdown=\"%s\" ", node->details->shutdown ? "true" : "false");
        fprintf(stream, "expected_up=\"%s\" ", node->details->expected_up ? "true" : "false");
        fprintf(stream, "is_dc=\"%s\" ", node->details->is_dc ? "true" : "false");
        fprintf(stream, "resources_running=\"%d\" ", g_list_length(node->details->running_rsc));
        fprintf(stream, "type=\"%s\" ", node_type);

        if (group_by_node) {
            GListPtr lpc2 = NULL;
            fprintf(stream, ">\n");
            for (lpc2 = node->details->running_rsc; lpc2 != NULL; lpc2 = lpc2->next) {
                resource_t *rsc = (resource_t *) lpc2->data;

                rsc->fns->print(rsc, "            ", pe_print_xml | pe_print_rsconly, stream);
            }
            fprintf(stream, "        </node>\n");
        } else {
            fprintf(stream, "/>\n");
        }
    }
    fprintf(stream, "    </nodes>\n");

     
    if (group_by_node == FALSE || inactive_resources) {
        fprintf(stream, "    <resources>\n");
        for (gIter = data_set->resources; gIter != NULL; gIter = gIter->next) {
            resource_t *rsc = (resource_t *) gIter->data;
            gboolean is_active = rsc->fns->active(rsc, TRUE);
            gboolean partially_active = rsc->fns->active(rsc, FALSE);

            if (is_set(rsc->flags, pe_rsc_orphan) && is_active == FALSE) {
                continue;

            } else if (group_by_node == FALSE) {
                if (partially_active || inactive_resources) {
                    rsc->fns->print(rsc, "        ", pe_print_xml, stream);
                }

            } else if (is_active == FALSE && inactive_resources) {
                rsc->fns->print(rsc, "        ", pe_print_xml, stream);
            }
        }
        fprintf(stream, "    </resources>\n");
    }

    fprintf(stream, "</crm_mon>\n");
    fflush(stream);
    fclose(stream);

    return 0;
}