static int printportlist(char *buf, int bufsize, struct portlist* pl, char * delim){
	int printed = 0;

	for(; pl; pl = pl->next){
		if(printed > (bufsize - 64)) break;
		if(pl->startport != pl->endport)
			printed += sprintf(buf+printed, "%hu-%hu%s", pl->startport, pl->endport, pl->next?delim:"");
		else {
 
			printed += sprintf(buf+printed, "%hu%s", pl->startport, pl->next?delim:"");
		}
		if(printed > (bufsize - 64)) {
			printed += sprintf(buf+printed, "...");
			break;
		}
	}
	return printed;
}
