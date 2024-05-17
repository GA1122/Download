FileTransfer::ExpandInputFileList( ClassAd *job, MyString &error_msg ) {


	MyString input_files;
	if( job->LookupString(ATTR_TRANSFER_INPUT_FILES,input_files) != 1 )
	{
		return true;  
	}

	MyString iwd;
	if( job->LookupString(ATTR_JOB_IWD,iwd) != 1 )
	{
		error_msg.sprintf("Failed to expand transfer input list because no IWD found in job ad.");
		return false;
	}

	MyString expanded_list;
	if( !FileTransfer::ExpandInputFileList(input_files.Value(),iwd.Value(),expanded_list,error_msg) )
	{
		return false;
	}

	if( expanded_list != input_files ) {
		dprintf(D_FULLDEBUG,"Expanded input file list: %s\n",expanded_list.Value());
		job->Assign(ATTR_TRANSFER_INPUT_FILES,expanded_list.Value());
	}
	return true;
}
