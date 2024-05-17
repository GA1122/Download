static bool hash_dma_valid_data(struct scatterlist *sg, int datasize)
{
	bool aligned;

	 
	if (hash_get_nents(sg, datasize, &aligned) < 1)
		return false;

	return aligned;
}
