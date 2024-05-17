void md_super_wait(struct mddev *mddev)
{
	 
	wait_event(mddev->sb_wait, atomic_read(&mddev->pending_writes)==0);
}
