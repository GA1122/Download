BaseShadow::publishShadowAttrs( ClassAd* ad )
{
	MyString tmp;
	tmp = ATTR_SHADOW_IP_ADDR;
	tmp += "=\"";
	tmp += daemonCore->InfoCommandSinfulString();
    tmp += '"';
	ad->Insert( tmp.Value() );

	tmp = ATTR_SHADOW_VERSION;
	tmp += "=\"";
	tmp += CondorVersion();
    tmp += '"';
	ad->Insert( tmp.Value() );

	char* my_uid_domain = param( "UID_DOMAIN" );
	if( my_uid_domain ) {
		tmp = ATTR_UID_DOMAIN;
		tmp += "=\"";
		tmp += my_uid_domain;
		tmp += '"';
		ad->Insert( tmp.Value() );
		free( my_uid_domain );
	}
}
