void add_disk_randomness(struct gendisk *disk)
{
	if (!disk || !disk->random)
		return;
	 
	DEBUG_ENT("disk event %d:%d\n",
		  MAJOR(disk_devt(disk)), MINOR(disk_devt(disk)));

	add_timer_randomness(disk->random, 0x100 + disk_devt(disk));
}
