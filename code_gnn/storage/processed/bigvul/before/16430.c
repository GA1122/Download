CStarter::publishJobInfoAd(List<UserProc>* proc_list, ClassAd* ad)
{
	bool found_one = false;
	if( pre_script && pre_script->PublishUpdateAd(ad) ) {
		found_one = true;
	}
	
	if (m_deferred_job_update)
	{
		MyString buf;
		buf.sprintf( "%s=\"Exited\"", ATTR_JOB_STATE );
	}
	
	UserProc *job;
	proc_list->Rewind();
	while ((job = proc_list->Next()) != NULL) {
		if( job->PublishUpdateAd(ad) ) {
			found_one = true;
		}
	}
	if( post_script && post_script->PublishUpdateAd(ad) ) {
		found_one = true;
	}
	return found_one;
}
