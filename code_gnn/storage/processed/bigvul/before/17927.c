XdmcpDecrementKey (XdmAuthKeyPtr key)
{
    int	i;

    i = 7;
    while (key->data[i]-- == 0)
	if (--i < 0)
	    break;
}
