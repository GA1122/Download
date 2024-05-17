OJPEGReadSkip(OJPEGState* sp, uint16 len)
{
	uint16 m;
	uint16 n;
	m=len;
	n=m;
	if (n>sp->in_buffer_togo)
		n=sp->in_buffer_togo;
	sp->in_buffer_cur+=n;
	sp->in_buffer_togo-=n;
	m-=n;
	if (m>0)
	{
		assert(sp->in_buffer_togo==0);
		n=m;
		if ((uint64)n>sp->in_buffer_file_togo)
			n=(uint16)sp->in_buffer_file_togo;
		sp->in_buffer_file_pos+=n;
		sp->in_buffer_file_togo-=n;
		sp->in_buffer_file_pos_log=0;
		 
	}
}
