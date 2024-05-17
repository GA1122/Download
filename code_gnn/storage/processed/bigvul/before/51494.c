static inline int vop_vdev_inited(struct vop_vdev *vdev)
{
	if (!vdev)
		return -EINVAL;
	 
	if (!vdev->dd || !vdev->dd->type) {
		dev_err(vop_dev(vdev), "%s %d err %d\n",
			__func__, __LINE__, -EINVAL);
		return -EINVAL;
	}
	 
	if (vdev->dd->type == -1) {
		dev_dbg(vop_dev(vdev), "%s %d err %d\n",
			__func__, __LINE__, -ENODEV);
		return -ENODEV;
	}
	return 0;
}
