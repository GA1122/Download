CronTab::contains( ExtArray<int> &list, const int &elt ) 
{
	bool ret = false;
	int ctr;
	for ( ctr = 0; ctr <= list.getlast(); ctr++ ) {
		if ( elt == list[ctr] ) {
			ret = true;
			break;	
		}
	}  
	return ( ret );
}
