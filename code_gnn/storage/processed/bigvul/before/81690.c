struct vbg_dev *vbg_get_gdev(void)
{
	mutex_lock(&vbg_gdev_mutex);

	 
	if (vbg_gdev)
		return vbg_gdev;

	mutex_unlock(&vbg_gdev_mutex);
	return ERR_PTR(-ENODEV);
}
