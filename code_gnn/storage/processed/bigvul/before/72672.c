int jas_setdbglevel(int dbglevel)
{
	int olddbglevel;

	 
	olddbglevel = jas_dbglevel;

	 
	jas_dbglevel = dbglevel;

	 
	return olddbglevel;
}
