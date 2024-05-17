convert_id_to_ns(FILE *idfile, unsigned int in_id)
{
	unsigned int nsuid,    
		     hostuid,  
		     count;    
	char line[400];
	int ret;

	fseek(idfile, 0L, SEEK_SET);
	while (fgets(line, 400, idfile)) {
		ret = sscanf(line, "%u %u %u\n", &nsuid, &hostuid, &count);
		if (ret != 3)
			continue;
		if (hostuid + count < hostuid || nsuid + count < nsuid) {
			 
			fprintf(stderr, "pid wrapparound at entry %u %u %u in %s\n",
				nsuid, hostuid, count, line);
			return -1;
		}
		if (hostuid <= in_id && hostuid+count > in_id) {
			 
			return (in_id - hostuid) + nsuid;
		}
	}

	return -1;
}
