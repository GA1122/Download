svc_create(struct svc_program *prog, unsigned int bufsize,
	   struct svc_serv_ops *ops)
{
	return __svc_create(prog, bufsize,  1, ops);
}
