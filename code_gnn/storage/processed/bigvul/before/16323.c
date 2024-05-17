BaseShadow::emailHoldEvent( const char* reason ) 
{
	Email mailer;
	mailer.sendHold( jobAd, reason );
}
