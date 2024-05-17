 GrubFS *grubfs_new (struct grub_fs *myfs, void *data) {
	struct grub_file *file;
	GrubFS *gfs = empty (sizeof (GrubFS));
	gfs->file = file = empty (sizeof (struct grub_file));
	file->device = empty (sizeof (struct grub_device)+1024);
	file->device->disk = empty (sizeof (struct grub_disk));
	file->device->disk->dev = (grub_disk_dev_t)file->device;  
	file->device->disk->dev->read = read_foo;  
	file->device->disk->data = data;
	file->fs = myfs;
	return gfs;
}
