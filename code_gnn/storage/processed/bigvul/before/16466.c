cluster_exists( int cluster )
{
	return proc_exists( cluster, -1 );
}
