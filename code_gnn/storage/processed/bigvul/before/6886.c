static int smp_conv_req_capture(const struct arg *args, struct sample *smp, void *private)
{
	struct proxy *fe = strm_fe(smp->strm);
	int idx, i;
	struct cap_hdr *hdr;
	int len;

	if (!args || args->type != ARGT_SINT)
		return 0;

	idx = args->data.sint;

	 
	if (idx > fe->nb_req_cap - 1)
		return 0;

	 
	for (hdr = fe->req_cap, i = fe->nb_req_cap - 1;
	     hdr != NULL && i != idx ;
	     i--, hdr = hdr->next);
	if (!hdr)
		return 0;

	 
	if (smp->strm->req_cap[hdr->index] == NULL)
		smp->strm->req_cap[hdr->index] = pool_alloc(hdr->pool);
	if (smp->strm->req_cap[hdr->index] == NULL)
		return 0;

	 
	len = smp->data.u.str.len;
	if (len > hdr->len)
		len = hdr->len;

	 
	memcpy(smp->strm->req_cap[idx], smp->data.u.str.str, len);
	smp->strm->req_cap[idx][len] = '\0';

	return 1;
}
