void svc_printk(struct svc_rqst *rqstp, const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	char 	buf[RPC_MAX_ADDRBUFLEN];

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	dprintk("svc: %s: %pV", svc_print_addr(rqstp, buf, sizeof(buf)), &vaf);

	va_end(args);
}
