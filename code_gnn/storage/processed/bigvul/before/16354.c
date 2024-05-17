BaseShadow::updateJobAttr( const char *name, const char *expr, bool log )
{
	return job_updater->updateAttr( name, expr, false, log );
}
