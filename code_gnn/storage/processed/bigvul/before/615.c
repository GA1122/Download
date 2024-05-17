struct asn1_data *asn1_init(TALLOC_CTX *mem_ctx)
{
	struct asn1_data *ret = talloc_zero(mem_ctx, struct asn1_data);
	if (ret == NULL) {
		DEBUG(0,("asn1_init failed! out of memory\n"));
	}
	return ret;
}
