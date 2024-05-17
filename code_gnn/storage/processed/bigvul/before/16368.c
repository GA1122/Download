pseudo_get_job_info(ClassAd *&ad, bool &delete_ad)
{
	ClassAd * the_ad;
	delete_ad = false;

	the_ad = thisRemoteResource->getJobAd();
	ASSERT( the_ad );

	thisRemoteResource->filetrans.Init( the_ad, false, PRIV_USER, false );

	std::string file;
	if ( the_ad->LookupString( ATTR_JOB_OUTPUT, file ) &&
		 strcmp( file.c_str(), StdoutRemapName ) ) {

		thisRemoteResource->filetrans.AddDownloadFilenameRemap( StdoutRemapName, file.c_str() );
	}
	if ( the_ad->LookupString( ATTR_JOB_ERROR, file ) &&
		 strcmp( file.c_str(), StderrRemapName ) ) {

		thisRemoteResource->filetrans.AddDownloadFilenameRemap( StderrRemapName, file.c_str() );
	}

	Shadow->publishShadowAttrs( the_ad );

	ad = the_ad;

	const CondorVersionInfo *vi = syscall_sock->get_peer_version();
	if ( vi && !vi->built_since_version(7,7,2) ) {
		std::string value;
		ad->LookupString( ATTR_SHOULD_TRANSFER_FILES, value );
		ShouldTransferFiles_t should_transfer = getShouldTransferFilesNum( value.c_str() );

		if ( should_transfer == STF_IF_NEEDED || should_transfer == STF_YES ) {
			ad = new ClassAd( *ad );
			delete_ad = true;

			bool stream;
			std::string stdout_name;
			std::string stderr_name;
			ad->LookupString( ATTR_JOB_OUTPUT, stdout_name );
			ad->LookupString( ATTR_JOB_ERROR, stderr_name );
			if ( ad->LookupBool( ATTR_STREAM_OUTPUT, stream ) && !stream &&
				 !nullFile( stdout_name.c_str() ) ) {
				ad->Assign( ATTR_JOB_OUTPUT, StdoutRemapName );
			}
			if ( ad->LookupBool( ATTR_STREAM_ERROR, stream ) && !stream &&
				 !nullFile( stderr_name.c_str() ) ) {
				if ( stdout_name == stderr_name ) {
					ad->Assign( ATTR_JOB_ERROR, StdoutRemapName );
				} else {
					ad->Assign( ATTR_JOB_ERROR, StderrRemapName );
				}
			}
		} else if ( should_transfer != STF_NO ) {
			dprintf( D_ALWAYS, "pseudo_get_job_info(): Unexpected value for %s: %s (%d)!\n",
					 ATTR_SHOULD_TRANSFER_FILES, value.c_str(),
					 should_transfer );
		}
	}

	return 0;
}
