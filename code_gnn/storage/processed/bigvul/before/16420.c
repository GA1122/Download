CStarter::exitAfterGlexec( int code )
{
	daemonCore->Proc_Family_Cleanup();

	chdir( "/" );
	char* glexec_dir_path = condor_dirname( Execute );
	ASSERT( glexec_dir_path );
	Directory glexec_dir( glexec_dir_path );
	glexec_dir.Remove_Entire_Directory();
	rmdir( glexec_dir_path );
	free( glexec_dir_path );

	exit( code );
}
