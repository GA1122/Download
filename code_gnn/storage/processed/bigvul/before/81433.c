trace_printk_seq(struct trace_seq *s)
{
	 
	if (s->seq.len >= TRACE_MAX_PRINT)
		s->seq.len = TRACE_MAX_PRINT;

	 
	if (WARN_ON_ONCE(s->seq.len >= s->seq.size))
		s->seq.len = s->seq.size - 1;

	 
	s->buffer[s->seq.len] = 0;

	printk(KERN_TRACE "%s", s->buffer);

	trace_seq_init(s);
}
