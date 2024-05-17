int fwriteAlice(void *clientData, const uint8_t *input, uint32_t size) {
	 
	my_Context_t *clientContext = (my_Context_t *)clientData;
	char *filename = clientContext->zidFilename;

	FILE *ALICECACHE = fopen(filename, "w+");
	int retval = fwrite(input, 1, size, ALICECACHE);
	fclose(ALICECACHE);
	return retval;
}