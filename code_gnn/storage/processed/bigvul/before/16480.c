touched_recently(char const *fname,time_t delta)
{
	StatInfo statinfo(fname);
	if( statinfo.Error() != 0 ) {
		return false;
	}
	if( abs((int)(time(NULL)-statinfo.GetModifyTime())) > delta ) {
		return false;
	}
	return true;
}
