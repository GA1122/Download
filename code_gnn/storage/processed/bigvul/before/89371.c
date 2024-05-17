int spl_load_simple_fit(struct spl_image_info *spl_image,
			struct spl_load_info *info, ulong sector, void *fit)
{
	int sectors;
	ulong size;
	unsigned long count;
	struct spl_image_info image_info;
	int node = -1;
	int images, ret;
	int base_offset, hsize, align_len = ARCH_DMA_MINALIGN - 1;
	int index = 0;
	int firmware_node;

	 
	size = fdt_totalsize(fit);
	size = (size + 3) & ~3;
	size = board_spl_fit_size_align(size);
	base_offset = (size + 3) & ~3;

	 
	hsize = (size + info->bl_len + align_len) & ~align_len;
	fit = spl_get_load_buffer(-hsize, hsize);
	sectors = get_aligned_image_size(info, size, 0);
	count = info->read(info, sector, sectors, fit);
	debug("fit read sector %lx, sectors=%d, dst=%p, count=%lu, size=0x%lx\n",
	      sector, sectors, fit, count, size);

	if (count == 0)
		return -EIO;

	 
	if (spl_load_simple_fit_skip_processing())
		return 0;

	 
	images = fdt_path_offset(fit, FIT_IMAGES_PATH);
	if (images < 0) {
		debug("%s: Cannot find /images node: %d\n", __func__, images);
		return -1;
	}

#ifdef CONFIG_SPL_FPGA_SUPPORT
	node = spl_fit_get_image_node(fit, images, "fpga", 0);
	if (node >= 0) {
		 
		ret = spl_load_fit_image(info, sector, fit, base_offset, node,
					 spl_image);
		if (ret) {
			printf("%s: Cannot load the FPGA: %i\n", __func__, ret);
			return ret;
		}

		debug("FPGA bitstream at: %x, size: %x\n",
		      (u32)spl_image->load_addr, spl_image->size);

		ret = fpga_load(0, (const void *)spl_image->load_addr,
				spl_image->size, BIT_FULL);
		if (ret) {
			printf("%s: Cannot load the image to the FPGA\n",
			       __func__);
			return ret;
		}

		puts("FPGA image loaded from FIT\n");
		node = -1;
	}
#endif

	 
	if (node < 0)
		node = spl_fit_get_image_node(fit, images, FIT_FIRMWARE_PROP,
					      0);
#ifdef CONFIG_SPL_OS_BOOT
	if (node < 0)
		node = spl_fit_get_image_node(fit, images, FIT_KERNEL_PROP, 0);
#endif
	if (node < 0) {
		debug("could not find firmware image, trying loadables...\n");
		node = spl_fit_get_image_node(fit, images, "loadables", 0);
		 
		index = 1;
	}
	if (node < 0) {
		debug("%s: Cannot find u-boot image node: %d\n",
		      __func__, node);
		return -1;
	}

	 
	ret = spl_load_fit_image(info, sector, fit, base_offset, node,
				 spl_image);
	if (ret)
		return ret;

	 
	if (!spl_fit_image_get_os(fit, node, &spl_image->os))
		debug("Image OS is %s\n", genimg_get_os_name(spl_image->os));
#if !defined(CONFIG_SPL_OS_BOOT)
	else
		spl_image->os = IH_OS_U_BOOT;
#endif

	 
	if (spl_image->os == IH_OS_U_BOOT)
		spl_fit_append_fdt(spl_image, info, sector, fit,
				   images, base_offset);

	firmware_node = node;
	 
	for (; ; index++) {
		uint8_t os_type = IH_OS_INVALID;

		node = spl_fit_get_image_node(fit, images, "loadables", index);
		if (node < 0)
			break;

		 
		if (firmware_node == node)
			continue;

		ret = spl_load_fit_image(info, sector, fit, base_offset, node,
					 &image_info);
		if (ret < 0)
			continue;

		if (!spl_fit_image_get_os(fit, node, &os_type))
			debug("Loadable is %s\n", genimg_get_os_name(os_type));
#if CONFIG_IS_ENABLED(FIT_IMAGE_TINY)
		else
			os_type = IH_OS_U_BOOT;
#endif

		if (os_type == IH_OS_U_BOOT) {
			spl_fit_append_fdt(&image_info, info, sector,
					   fit, images, base_offset);
			spl_image->fdt_addr = image_info.fdt_addr;
		}

		 
		if (spl_image->entry_point == FDT_ERROR &&
		    image_info.entry_point != FDT_ERROR)
			spl_image->entry_point = image_info.entry_point;

		 
		if (spl_image->fdt_addr)
			spl_fit_record_loadable(fit, images, index,
						spl_image->fdt_addr,
						&image_info);
	}

	 
	if (spl_image->entry_point == FDT_ERROR || spl_image->entry_point == 0)
		spl_image->entry_point = spl_image->load_addr;

	spl_image->flags |= SPL_FIT_FOUND;

#ifdef CONFIG_IMX_HAB
	board_spl_fit_post_load((ulong)fit, size);
#endif

	return 0;
}
