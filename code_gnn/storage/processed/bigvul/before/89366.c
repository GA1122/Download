static int spl_fit_append_fdt(struct spl_image_info *spl_image,
			      struct spl_load_info *info, ulong sector,
			      void *fit, int images, ulong base_offset)
{
	struct spl_image_info image_info;
	int node, ret = 0, index = 0;

	 
	image_info.load_addr = spl_image->load_addr + spl_image->size;

	 
	node = spl_fit_get_image_node(fit, images, FIT_FDT_PROP, index++);
	if (node < 0) {
		debug("%s: cannot find FDT node\n", __func__);

		 
		if (gd->fdt_blob)
			memcpy((void *)image_info.load_addr, gd->fdt_blob,
			       fdt_totalsize(gd->fdt_blob));
		else
			return node;
	} else {
		ret = spl_load_fit_image(info, sector, fit, base_offset, node,
					 &image_info);
		if (ret < 0)
			return ret;
	}

	 
	spl_image->fdt_addr = (void *)image_info.load_addr;
#if !CONFIG_IS_ENABLED(FIT_IMAGE_TINY)
	if (CONFIG_IS_ENABLED(LOAD_FIT_APPLY_OVERLAY)) {
		void *tmpbuffer = NULL;

		for (; ; index++) {
			node = spl_fit_get_image_node(fit, images, FIT_FDT_PROP,
						      index);
			if (node == -E2BIG) {
				debug("%s: No additional FDT node\n", __func__);
				break;
			} else if (node < 0) {
				debug("%s: unable to find FDT node %d\n",
				      __func__, index);
				continue;
			}

			if (!tmpbuffer) {
				 
				tmpbuffer = malloc(CONFIG_SPL_LOAD_FIT_APPLY_OVERLAY_BUF_SZ);
				if (!tmpbuffer)
					debug("%s: unable to allocate space for overlays\n",
					      __func__);
			}
			image_info.load_addr = (ulong)tmpbuffer;
			ret = spl_load_fit_image(info, sector, fit, base_offset,
						 node, &image_info);
			if (ret < 0)
				break;

			 
			ret = fdt_increase_size(spl_image->fdt_addr,
						image_info.size);
			if (ret < 0)
				break;

			ret = fdt_overlay_apply_verbose(spl_image->fdt_addr,
							(void *)image_info.load_addr);
			if (ret) {
				pr_err("failed to apply DT overlay %s\n",
				       fit_get_name(fit, node, NULL));
				break;
			}

			debug("%s: DT overlay %s applied\n", __func__,
			      fit_get_name(fit, node, NULL));
		}
		if (tmpbuffer)
			free(tmpbuffer);
		if (ret)
			return ret;
	}
	 
	ret = fdt_shrink_to_minimum(spl_image->fdt_addr, 8192);
	if (ret < 0)
		return ret;
#endif

	return ret;
}
