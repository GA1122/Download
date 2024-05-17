static int run_userns_fn(void *data)
{
	struct userns_fn_data *d = data;
	char c;

	close(d->p[1]);
	if (read(d->p[0], &c, 1) != 1)
		return -1;
	close(d->p[0]);
	return d->fn(d->arg);
}
