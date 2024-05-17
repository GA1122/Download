CStarter::~CStarter()
{
	if( Execute ) {
		free(Execute);
	}
	if( orig_cwd ) {
		free(orig_cwd);
	}
	if( jic ) {
		delete jic;
	}
	if( pre_script ) {
		delete( pre_script );
	}
	if( post_script ) {
		delete( post_script );
	}
}
