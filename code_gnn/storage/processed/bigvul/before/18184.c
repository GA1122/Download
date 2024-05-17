static void irssi_ssl_free(GIOChannel *handle)
{
	GIOSSLChannel *chan = (GIOSSLChannel *)handle;
	g_io_channel_unref(chan->giochan);
	SSL_free(chan->ssl);
	if (chan->ctx != ssl_ctx)
		SSL_CTX_free(chan->ctx);
 	g_free(chan);
 }