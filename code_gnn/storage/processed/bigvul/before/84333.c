UNCURL_EXPORT struct uncurl_conn *uncurl_new_conn(struct uncurl_conn *parent)
{
	struct uncurl_conn *ucc = calloc(1, sizeof(struct uncurl_conn));

	if (parent) {
		memcpy(&ucc->opts, &parent->opts, sizeof(struct uncurl_opts));
		memcpy(&ucc->nopts, &parent->nopts, sizeof(struct net_opts));
		memcpy(&ucc->topts, &parent->topts, sizeof(struct tls_opts));

	} else {
		uncurl_default_opts(&ucc->opts);
		net_default_opts(&ucc->nopts);
		tls_default_opts(&ucc->topts);
	}

	ucc->seed = ws_rand(&ucc->seed);

	return ucc;
}
