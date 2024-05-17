static int nfs_lseek(struct device_d *dev, FILE *file, loff_t pos)
{
	struct file_priv *priv = file->priv;

	kfifo_reset(priv->fifo);

	return 0;
}
