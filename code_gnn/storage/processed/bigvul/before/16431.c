CStarter::publishPostScriptUpdateAd( ClassAd* ad )
{
	if( post_script && post_script->PublishUpdateAd(ad) ) {
		return true;
	}
	return false;
}
