void vnc_jobs_consume_buffer(VncState *vs)
{
    bool flush;

    vnc_lock_output(vs);
    if (vs->jobs_buffer.offset) {
        vnc_write(vs, vs->jobs_buffer.buffer, vs->jobs_buffer.offset);
        buffer_reset(&vs->jobs_buffer);
    }
    flush = vs->csock != -1 && vs->abort != true;
    vnc_unlock_output(vs);

    if (flush) {
      vnc_flush(vs);
    }
}
