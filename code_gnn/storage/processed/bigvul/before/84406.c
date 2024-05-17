got_buffer_from_side (ProxySide *side, Buffer *buffer)
{
  FlatpakProxyClient *client = side->client;

  if (side == &client->client_side)
    got_buffer_from_client (client, side, buffer);
  else
     got_buffer_from_bus (client, side, buffer);
 }