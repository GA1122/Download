BaseShadow::CommitSuspensionTime(ClassAd *jobAd)
{
	int uncommitted_suspension_time = 0;
	jobAd->LookupInteger(ATTR_UNCOMMITTED_SUSPENSION_TIME,uncommitted_suspension_time);
	if( uncommitted_suspension_time > 0 ) {
		int committed_suspension_time = 0;
		jobAd->LookupInteger( ATTR_COMMITTED_SUSPENSION_TIME,
							  committed_suspension_time );
		committed_suspension_time += uncommitted_suspension_time;
		jobAd->Assign( ATTR_COMMITTED_SUSPENSION_TIME, committed_suspension_time );
		jobAd->Assign( ATTR_UNCOMMITTED_SUSPENSION_TIME, 0 );
	}
}
