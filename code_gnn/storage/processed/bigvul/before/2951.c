pdf14_find_backdrop_buf(pdf14_ctx *ctx)
{
    pdf14_buf *buf = ctx->stack;

    while (buf != NULL) {
        if (buf->isolated) return NULL;
        if (buf->knockout && !buf->isolated) return buf->saved;
         
        if (buf->saved != NULL && buf->saved->knockout && buf->saved->isolated) return NULL;
         
        if (buf->saved != NULL && buf->saved->knockout && !buf->saved->isolated)
            return NULL;
         
        if (!buf->knockout) return buf->saved;
        buf = buf->saved;
    }
     
    return NULL;
}
