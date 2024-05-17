void esp_command_complete(SCSIRequest *req, uint32_t status,
                                 size_t resid)
{
    ESPState *s = req->hba_private;

    trace_esp_command_complete();
    if (s->ti_size != 0) {
        trace_esp_command_complete_unexpected();
    }
    s->ti_size = 0;
    s->dma_left = 0;
    s->async_len = 0;
    if (status) {
        trace_esp_command_complete_fail();
    }
    s->status = status;
    s->rregs[ESP_RSTAT] = STAT_ST;
    esp_dma_done(s);
    if (s->current_req) {
        scsi_req_unref(s->current_req);
        s->current_req = NULL;
        s->current_dev = NULL;
    }
}
