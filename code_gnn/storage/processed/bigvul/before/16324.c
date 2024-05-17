BaseShadow::emailRemoveEvent( const char* reason ) 
{
	Email mailer;
	mailer.sendRemove( jobAd, reason );
}
