BaseShadow::updateJobInQueue( update_t type )
{
	MyString buf;
	buf.sprintf( "%s = %f", ATTR_BYTES_SENT, (prev_run_bytes_sent +
											  bytesReceived()) );
	jobAd->Insert( buf.Value() );
	buf.sprintf( "%s = %f", ATTR_BYTES_RECVD, (prev_run_bytes_recvd +
											   bytesSent()) );
	jobAd->Insert( buf.Value() );

	ASSERT( job_updater );

	return job_updater->updateJob( type );
}
