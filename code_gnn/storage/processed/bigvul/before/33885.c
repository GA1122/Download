construct_pam_passwd(int num_msg, const struct pam_message **msg,
                     struct pam_response **response, void *data)
{
    int count = 0;
    struct pam_response *reply;
    char *string = (char *)data;

    CRM_CHECK(data, return PAM_CONV_ERR);
    CRM_CHECK(num_msg == 1, return PAM_CONV_ERR);        

    reply = calloc(1, sizeof(struct pam_response));
    CRM_ASSERT(reply != NULL);

    for (count = 0; count < num_msg; ++count) {
        switch (msg[count]->msg_style) {
            case PAM_TEXT_INFO:
                crm_info("PAM: %s\n", msg[count]->msg);
                break;
            case PAM_PROMPT_ECHO_OFF:
            case PAM_PROMPT_ECHO_ON:
                reply[count].resp_retcode = 0;
                reply[count].resp = string;      
            case PAM_ERROR_MSG:
                 
                 
                break;
            default:
                crm_err("Unhandled conversation type: %d", msg[count]->msg_style);
                goto bail;
        }
    }

    *response = reply;
    reply = NULL;

    return PAM_SUCCESS;

  bail:
    for (count = 0; count < num_msg; ++count) {
        if (reply[count].resp != NULL) {
            switch (msg[count]->msg_style) {
                case PAM_PROMPT_ECHO_ON:
                case PAM_PROMPT_ECHO_OFF:
                     
                    while (*(reply[count].resp)) {
                        *(reply[count].resp)++ = '\0';
                    }
                    free(reply[count].resp);
                    break;
            }
            reply[count].resp = NULL;
        }
    }
    free(reply);
    reply = NULL;

    return PAM_CONV_ERR;
}
