cib_remote_free(cib_t * cib)
{
    int rc = pcmk_ok;

    crm_warn("Freeing CIB");
    if (cib->state != cib_disconnected) {
        rc = cib_remote_signoff(cib);
        if (rc == pcmk_ok) {
            cib_remote_opaque_t *private = cib->variant_opaque;

            free(private->server);
            free(private->user);
            free(private->passwd);
            free(cib->cmds);
            free(private);
            free(cib);
        }
    }

     return rc;
 }