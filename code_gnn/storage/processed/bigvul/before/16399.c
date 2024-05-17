CStarter::RemoteSuspend(int)
{
	int retval = this->Suspend();


	jic->Suspend();
	return retval;
}
