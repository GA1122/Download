void mptsas_reply(MPTSASState *s, MPIDefaultReply *reply)
{
    if (s->doorbell_state == DOORBELL_WRITE) {
         
        s->doorbell_state = DOORBELL_READ;
        s->doorbell_reply_idx = 0;
        s->doorbell_reply_size = reply->MsgLength * 2;
        memcpy(s->doorbell_reply, reply, s->doorbell_reply_size * 2);
        s->intr_status |= MPI_HIS_DOORBELL_INTERRUPT;
        mptsas_update_interrupt(s);
    } else {
        mptsas_post_reply(s, reply);
    }
}