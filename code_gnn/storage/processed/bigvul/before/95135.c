static void cmd_close(char *tag, char *cmd)
{
    if (backend_current) {
         
        prot_printf(backend_current->out, "%s %s\r\n", tag, cmd);
         
        pipe_including_tag(backend_current, tag, 0);

         
        protgroup_delete(protin, backend_current->in);

        backend_current = NULL;
        return;
    }

     
    if ((cmd[0] == 'C') && index_hasrights(imapd_index, ACL_EXPUNGE)) {
        index_expunge(imapd_index, NULL, 1);
         
    }

    index_close(&imapd_index);

     
    prot_printf(imapd_out, "%s OK %s\r\n",
                tag, error_message(IMAP_OK_COMPLETED));
}
