CStarter::SpawnPreScript( void )
{
	if ( this->deferral_tid != -1 ) {
		this->deferral_tid = -1;
	}
	
	ClassAd* jobAd = jic->jobClassAd();
	char* tmp = NULL;
	MyString attr;

	attr = "Pre";
	attr += ATTR_JOB_CMD;
	if( jobAd->LookupString(attr.Value(), &tmp) ) {
		free( tmp );
		tmp = NULL;
		pre_script = new ScriptProc( jobAd, "Pre" );
	}

	attr = "Post";
	attr += ATTR_JOB_CMD;
	if( jobAd->LookupString(attr.Value(), &tmp) ) {
		free( tmp );
		tmp = NULL;
		post_script = new ScriptProc( jobAd, "Post" );
	}

	if( pre_script ) {

		if( pre_script->StartJob() ) {
			return;
		} else {
			dprintf( D_ALWAYS, "Failed to start prescript, exiting\n" );
			main_shutdown_fast();
			return;
		}
	}

	if( ! SpawnJob() ) {
		dprintf( D_ALWAYS, "Failed to start main job, exiting.\n" );
		main_shutdown_fast();
	}
}
