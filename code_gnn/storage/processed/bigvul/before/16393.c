CStarter::RemoteContinue(int)
{
	int retval = this->Continue();


	jic->Continue();
	return retval;
}
