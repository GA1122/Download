layout_show(struct mddev *mddev, char *page)
{
	 
	if (mddev->reshape_position != MaxSector &&
	    mddev->layout != mddev->new_layout)
		return sprintf(page, "%d (%d)\n",
			       mddev->new_layout, mddev->layout);
	return sprintf(page, "%d\n", mddev->layout);
}