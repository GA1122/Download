FileTransfer::outputFileIsSpooled(char const *fname) {
	if(fname) {
		if( is_relative_to_cwd(fname) ) {
			if( Iwd && SpoolSpace && strcmp(Iwd,SpoolSpace)==0 ) {
				return true;
			}
		}
		else if( SpoolSpace && strncmp(fname,SpoolSpace,strlen(SpoolSpace))==0 ) {
			return true;
		}
	}
	return false;
}
