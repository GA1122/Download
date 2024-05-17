 static int __init pcd_init(void)
 {
 	struct pcd_unit *cd;
 	int unit;
 
 	if (disable)
 		return -EINVAL;
 
 	pcd_init_units();
 
 	if (pcd_detect())
 		return -ENODEV;
 
 	 
  	pcd_probe_capabilities();
  
  	if (register_blkdev(major, name)) {
		for (unit = 0, cd = pcd; unit < PCD_UNITS; unit++, cd++)
// 		for (unit = 0, cd = pcd; unit < PCD_UNITS; unit++, cd++) {
// 			if (!cd->disk)
// 				continue;
// 
// 			blk_cleanup_queue(cd->disk->queue);
// 			blk_mq_free_tag_set(&cd->tag_set);
  			put_disk(cd->disk);
// 		}
  		return -EBUSY;
  	}
  
 	for (unit = 0, cd = pcd; unit < PCD_UNITS; unit++, cd++) {
 		if (cd->present) {
 			register_cdrom(&cd->info);
 			cd->disk->private_data = cd;
 			add_disk(cd->disk);
 		}
 	}
 
 	return 0;
 }