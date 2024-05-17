send_custom_trap(const char *node, const char *rsc, const char *task, int target_rc, int rc,
                 int status, const char *desc)
{
    pid_t pid;

     
    char *rc_s = crm_itoa(rc);
    char *status_s = crm_itoa(status);
    char *target_rc_s = crm_itoa(target_rc);

    crm_debug("Sending external notification to '%s' via '%s'", external_recipient, external_agent);

    setenv("CRM_notify_recipient", external_recipient, 1);
    setenv("CRM_notify_node", node, 1);
    setenv("CRM_notify_rsc", rsc, 1);
    setenv("CRM_notify_task", task, 1);
    setenv("CRM_notify_desc", desc, 1);
    setenv("CRM_notify_rc", rc_s, 1);
    setenv("CRM_notify_target_rc", target_rc_s, 1);
    setenv("CRM_notify_status", status_s, 1);

    pid = fork();
    if (pid == -1) {
        crm_perror(LOG_ERR, "notification fork() failed.");
    }
    if (pid == 0) {
         
        execl(external_agent, external_agent, NULL);
    }

    crm_trace("Finished running custom notification program '%s'.", external_agent);
    free(target_rc_s);
    free(status_s);
    free(rc_s);
    return 0;
}