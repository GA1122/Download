 destroy_one_secret (gpointer data)
 {
  	char *secret = (char *) data;
  
  	 
g_message ("%s: destroying %s", __func__, secret);
  	memset (secret, 0, strlen (secret));
  	g_free (secret);
  }