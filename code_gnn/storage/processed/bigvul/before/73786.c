int floadBob(void *clientData, uint8_t **output, uint32_t *size, zrtpFreeBuffer_callback *cb) {
	 
	my_Context_t *clientContext = (my_Context_t *)clientData;
	char *filename = clientContext->zidFilename;

	FILE *BOBCACHE = fopen(filename, "r+");
	fseek(BOBCACHE, 0L, SEEK_END);   
  	*size = ftell(BOBCACHE);      
  	rewind(BOBCACHE);                
	*output = (uint8_t *)malloc(*size*sizeof(uint8_t)+1);
	if (fread(*output, 1, *size, BOBCACHE)==0){
		fprintf(stderr,"floadBob(): fread error.\n");
		return -1;
	}
	*(*output+*size) = '\0';
	*size += 1;
	fclose(BOBCACHE);
	*cb=freeBuf;
	return *size;
}
