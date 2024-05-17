print_html_status(pe_working_set_t * data_set, const char *filename, gboolean web_cgi)
{
    FILE *stream;
    GListPtr gIter = NULL;
    node_t *dc = NULL;
    static int updates = 0;
    char *filename_tmp = NULL;

    if (web_cgi) {
        stream = stdout;
        fprintf(stream, "Content-type: text/html\n\n");

    } else {
        filename_tmp = crm_concat(filename, "tmp", '.');
        stream = fopen(filename_tmp, "w");
        if (stream == NULL) {
            crm_perror(LOG_ERR, "Cannot open %s for writing", filename_tmp);
            free(filename_tmp);
            return -1;
        }
    }

    updates++;
    dc = data_set->dc_node;

    fprintf(stream, "<html>");
    fprintf(stream, "<head>");
    fprintf(stream, "<title>Cluster status</title>");
 
    fprintf(stream, "<meta http-equiv=\"refresh\" content=\"%d\">", reconnect_msec / 1000);
    fprintf(stream, "</head>");

     

    fprintf(stream, "<h2>Cluster summary</h2>");
    {
        char *now_str = NULL;
        time_t now = time(NULL);

        now_str = ctime(&now);
        now_str[24] = EOS;       
        fprintf(stream, "Last updated: <b>%s</b><br/>\n", now_str);
    }

    if (dc == NULL) {
        fprintf(stream, "Current DC: <font color=\"red\"><b>NONE</b></font><br/>");
    } else {
        fprintf(stream, "Current DC: %s (%s)<br/>", dc->details->uname, dc->details->id);
    }
    fprintf(stream, "%d Nodes configured.<br/>", g_list_length(data_set->nodes));
    fprintf(stream, "%d Resources configured.<br/>", count_resources(data_set, NULL));

     

    fprintf(stream, "<h3>Config Options</h3>\n");

    fprintf(stream, "<table>\n");
    fprintf(stream, "<tr><td>STONITH of failed nodes</td><td>:</td><td>%s</td></tr>\n",
            is_set(data_set->flags, pe_flag_stonith_enabled) ? "enabled" : "disabled");

    fprintf(stream, "<tr><td>Cluster is</td><td>:</td><td>%ssymmetric</td></tr>\n",
            is_set(data_set->flags, pe_flag_symmetric_cluster) ? "" : "a-");

    fprintf(stream, "<tr><td>No Quorum Policy</td><td>:</td><td>");
    switch (data_set->no_quorum_policy) {
        case no_quorum_freeze:
            fprintf(stream, "Freeze resources");
            break;
        case no_quorum_stop:
            fprintf(stream, "Stop ALL resources");
            break;
        case no_quorum_ignore:
            fprintf(stream, "Ignore");
            break;
        case no_quorum_suicide:
            fprintf(stream, "Suicide");
            break;
    }
    fprintf(stream, "\n</td></tr>\n</table>\n");

     

    fprintf(stream, "<h2>Node List</h2>\n");
    fprintf(stream, "<ul>\n");
    for (gIter = data_set->nodes; gIter != NULL; gIter = gIter->next) {
        node_t *node = (node_t *) gIter->data;

        fprintf(stream, "<li>");
        if (node->details->standby_onfail && node->details->online) {
            fprintf(stream, "Node: %s (%s): %s", node->details->uname, node->details->id,
                    "<font color=\"orange\">standby (on-fail)</font>\n");
        } else if (node->details->standby && node->details->online) {
            fprintf(stream, "Node: %s (%s): %s", node->details->uname, node->details->id,
                    "<font color=\"orange\">standby</font>\n");
        } else if (node->details->standby) {
            fprintf(stream, "Node: %s (%s): %s", node->details->uname, node->details->id,
                    "<font color=\"red\">OFFLINE (standby)</font>\n");
        } else if (node->details->online) {
            fprintf(stream, "Node: %s (%s): %s", node->details->uname, node->details->id,
                    "<font color=\"green\">online</font>\n");
        } else {
            fprintf(stream, "Node: %s (%s): %s", node->details->uname, node->details->id,
                    "<font color=\"red\">OFFLINE</font>\n");
        }
        if (group_by_node) {
            GListPtr lpc2 = NULL;

            fprintf(stream, "<ul>\n");
            for (lpc2 = node->details->running_rsc; lpc2 != NULL; lpc2 = lpc2->next) {
                resource_t *rsc = (resource_t *) lpc2->data;

                fprintf(stream, "<li>");
                rsc->fns->print(rsc, NULL, pe_print_html | pe_print_rsconly, stream);
                fprintf(stream, "</li>\n");
            }
            fprintf(stream, "</ul>\n");
        }
        fprintf(stream, "</li>\n");
    }
    fprintf(stream, "</ul>\n");

    if (group_by_node && inactive_resources) {
        fprintf(stream, "<h2>Inactive Resources</h2>\n");

    } else if (group_by_node == FALSE) {
        fprintf(stream, "<h2>Resource List</h2>\n");
    }

    if (group_by_node == FALSE || inactive_resources) {
        for (gIter = data_set->resources; gIter != NULL; gIter = gIter->next) {
            resource_t *rsc = (resource_t *) gIter->data;
            gboolean is_active = rsc->fns->active(rsc, TRUE);
            gboolean partially_active = rsc->fns->active(rsc, FALSE);

            if (is_set(rsc->flags, pe_rsc_orphan) && is_active == FALSE) {
                continue;

            } else if (group_by_node == FALSE) {
                if (partially_active || inactive_resources) {
                    rsc->fns->print(rsc, NULL, pe_print_html, stream);
                }

            } else if (is_active == FALSE && inactive_resources) {
                rsc->fns->print(rsc, NULL, pe_print_html, stream);
            }
        }
    }

    fprintf(stream, "</html>");
    fflush(stream);
    fclose(stream);

    if (!web_cgi) {
        if (rename(filename_tmp, filename) != 0) {
            crm_perror(LOG_ERR, "Unable to rename %s->%s", filename_tmp, filename);
        }
        free(filename_tmp);
    }
    return 0;
}