void vivid_fb_release_buffers(struct vivid_dev *dev)
{
	if (dev->video_vbase == NULL)
		return;

	 
	if (dev->fb_info.cmap.len)
		fb_dealloc_cmap(&dev->fb_info.cmap);

	 
	kfree(dev->fb_info.pseudo_palette);
	kfree((void *)dev->video_vbase);
}
