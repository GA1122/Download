static void disk_release_events(struct gendisk *disk)
{
	 
	WARN_ON_ONCE(disk->ev && disk->ev->block != 1);
	kfree(disk->ev);
}
