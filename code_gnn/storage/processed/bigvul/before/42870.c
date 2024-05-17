static gint select_next_page_no(gint current_page_no, gpointer data)
{
    GtkWidget *page;

 again:
    log_notice("%s: current_page_no:%d", __func__, current_page_no);
    current_page_no++;
    page = gtk_notebook_get_nth_page(g_assistant, current_page_no);

    if (pages[PAGENO_EVENT_SELECTOR].page_widget == page)
    {
        if (!g_expert_mode && (g_auto_event_list == NULL))
        {
            return current_page_no;  
        }

        if (!g_expert_mode)
        {
             
            char *event = setup_next_processed_event(&g_auto_event_list);
            if (!event)
            {
                current_page_no = pages[PAGENO_EVENT_PROGRESS].page_no - 1;
                goto again;
            }

            if (!get_sensitive_data_permission(event))
            {
                free(event);

                cancel_processing(g_lbl_event_log,   NULL, TERMINATE_NOFLAGS);
                current_page_no = pages[PAGENO_EVENT_PROGRESS].page_no - 1;
                goto again;
            }

            if (problem_data_get_content_or_NULL(g_cd, FILENAME_NOT_REPORTABLE))
            {
                free(event);

                char *msg = xasprintf(_("This problem should not be reported "
                                "(it is likely a known problem). %s"),
                                problem_data_get_content_or_NULL(g_cd, FILENAME_NOT_REPORTABLE)
                );
                cancel_processing(g_lbl_event_log, msg, TERMINATE_NOFLAGS);
                free(msg);
                current_page_no = pages[PAGENO_EVENT_PROGRESS].page_no - 1;
                goto again;
            }

            g_event_selected = event;

             
             
            check_event_config(g_event_selected);

             
            current_page_no = pages[PAGENO_EVENT_SELECTOR].page_no + 1;
            goto event_was_selected;
        }
    }

    if (pages[PAGENO_EVENT_SELECTOR + 1].page_widget == page)
    {
 event_was_selected:
        if (!g_event_selected)
        {
             
            current_page_no = pages[PAGENO_EVENT_SELECTOR].page_no - 1;
            goto again;
        }

        if (!event_need_review(g_event_selected))
        {
            current_page_no = pages[PAGENO_EVENT_PROGRESS].page_no - 1;
            goto again;
        }
    }

#if 0
    if (pages[PAGENO_EDIT_COMMENT].page_widget == page)
    {
        if (problem_data_get_content_or_NULL(g_cd, FILENAME_COMMENT))
            goto again;  
    }
#endif

    if (pages[PAGENO_EVENT_DONE].page_widget == page)
    {
        if (g_auto_event_list)
        {
             
            current_page_no = pages[PAGENO_SUMMARY].page_no;
        }
        goto again;
    }

    if (pages[PAGENO_NOT_SHOWN].page_widget == page)
    {
        if (!g_expert_mode)
            exit(0);
         
         
        current_page_no = pages[PAGENO_EVENT_SELECTOR].page_no - 1;
        goto again;
    }

    log_notice("%s: selected page #%d", __func__, current_page_no);
    return current_page_no;
}
