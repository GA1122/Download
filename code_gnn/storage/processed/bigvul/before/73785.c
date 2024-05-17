int floadAlice(void *clientData, uint8_t **output, uint32_t *size, zrtpFreeBuffer_callback *cb) {
	 
	my_Context_t *clientContext = (my_Context_t *)clientData;
	char *filename = clientContext->zidFilename;
	FILE *ALICECACHE = fopen(filename, "r+");
	fseek(ALICECACHE, 0L, SEEK_END);   
  	*size = ftell(ALICECACHE);      
  	rewind(ALICECACHE);                
	*output = (uint8_t *)malloc(*size*sizeof(uint8_t)+1);
	if (fread(*output, 1, *size, ALICECACHE)==0){
		fprintf(stderr,"floadAlice() fread() error\n");
	}
	*(*output+*size) = '\0';
	*size += 1;
	fclose(ALICECACHE);
	*cb=freeBuf;
	return *size;
}