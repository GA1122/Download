static int get_aligned_image_overhead(struct spl_load_info *info, int offset)
{
	 
	if (info->filename)
		return offset & (ARCH_DMA_MINALIGN - 1);

	return offset % info->bl_len;
}
