mon_refresh_display(gpointer user_data)
{
    xmlNode *cib_copy = copy_xml(current_cib);
    pe_working_set_t data_set;

    last_refresh = time(NULL);

    if (cli_config_update(&cib_copy, NULL, FALSE) == FALSE) {
        if (cib) {
            cib->cmds->signoff(cib);
        }
        print_as("Upgrade failed: %s", pcmk_strerror(-pcmk_err_dtd_validation));
        if (as_console) {
            sleep(2);
        }
        clean_up(EX_USAGE);
        return FALSE;
    }

    set_working_set_defaults(&data_set);
    data_set.input = cib_copy;
    cluster_status(&data_set);

    if (as_html_file || web_cgi) {
        if (print_html_status(&data_set, as_html_file, web_cgi) != 0) {
            fprintf(stderr, "Critical: Unable to output html file\n");
            clean_up(EX_USAGE);
        }
    } else if (as_xml) {
        if (print_xml_status(&data_set) != 0) {
            fprintf(stderr, "Critical: Unable to output xml file\n");
            clean_up(EX_USAGE);
        }
    } else if (daemonize) {
         

    } else if (simple_status) {
        print_simple_status(&data_set);
        if (has_warnings) {
            clean_up(EX_USAGE);
        }

    } else {
        print_status(&data_set);
    }

    cleanup_calculations(&data_set);
    return TRUE;
}