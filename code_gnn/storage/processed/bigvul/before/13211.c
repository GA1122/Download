nsPluginInstance::WriteReady(NPStream*   )
{
    if ( _streamfd != -1 ) {
	return 1024;
    } else {
	return 0;
    }
}
