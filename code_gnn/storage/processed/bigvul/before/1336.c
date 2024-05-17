static void nfs_do_close(struct file_priv *priv)
{
	if (priv->fifo)
		kfifo_free(priv->fifo);

	free(priv);
}
