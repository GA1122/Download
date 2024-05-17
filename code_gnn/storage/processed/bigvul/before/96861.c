void pipe_buf_mark_unmergeable(struct pipe_buffer *buf)
{
	if (buf->ops == &anon_pipe_buf_ops)
		buf->ops = &anon_pipe_buf_nomerge_ops;
}
