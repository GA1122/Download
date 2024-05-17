static int sample_conv_url_dec(const struct arg *args, struct sample *smp, void *private)
{
	 
	if (smp->flags & SMP_F_CONST || smp->data.u.str.size <= smp->data.u.str.len) {
		struct chunk *str = get_trash_chunk();
		memcpy(str->str, smp->data.u.str.str, smp->data.u.str.len);
		smp->data.u.str.str = str->str;
		smp->data.u.str.size = str->size;
		smp->flags &= ~SMP_F_CONST;
	}

	 
	smp->data.u.str.str[smp->data.u.str.len] = '\0';
	smp->data.u.str.len = url_decode(smp->data.u.str.str);
	return (smp->data.u.str.len >= 0);
}
