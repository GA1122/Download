static void __init early_reserve_initrd(void)
{
	 
	u64 ramdisk_image = get_ramdisk_image();
	u64 ramdisk_size  = get_ramdisk_size();
	u64 ramdisk_end   = PAGE_ALIGN(ramdisk_image + ramdisk_size);

	if (!boot_params.hdr.type_of_loader ||
	    !ramdisk_image || !ramdisk_size)
		return;		 

	memblock_reserve(ramdisk_image, ramdisk_end - ramdisk_image);
}
