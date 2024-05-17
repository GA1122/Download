static void *pppol2tp_seq_start(struct seq_file *m, loff_t *offs)
{
	struct pppol2tp_seq_data *pd = SEQ_START_TOKEN;
	loff_t pos = *offs;
	struct net *net;

	if (!pos)
		goto out;

	BUG_ON(m->private == NULL);
	pd = m->private;
	net = seq_file_net(m);

	if (pd->tunnel == NULL)
		pppol2tp_next_tunnel(net, pd);
	else
		pppol2tp_next_session(net, pd);

	 
	if ((pd->tunnel == NULL) && (pd->session == NULL))
		pd = NULL;

out:
	return pd;
}