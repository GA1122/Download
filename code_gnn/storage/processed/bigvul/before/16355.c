BaseShadow::updateJobAttr( const char *name, int value, bool log )
{
	return job_updater->updateAttr( name, value, false, log );
}
