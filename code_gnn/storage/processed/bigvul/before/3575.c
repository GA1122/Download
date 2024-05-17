msg_read (int fd)
{
  struct msg *msg;
  struct apimsghdr hdr;
  u_char buf[OSPF_API_MAX_MSG_SIZE];
  int bodylen;
  int rlen;

   
  rlen = readn (fd, (u_char *) &hdr, sizeof (struct apimsghdr));

  if (rlen < 0)
    {
      zlog_warn ("msg_read: readn %s", safe_strerror (errno));
      return NULL;
    }
  else if (rlen == 0)
    {
      zlog_warn ("msg_read: Connection closed by peer");
      return NULL;
    }
  else if (rlen != sizeof (struct apimsghdr))
    {
      zlog_warn ("msg_read: Cannot read message header!");
      return NULL;
    }

   
  if (hdr.version != OSPF_API_VERSION)
    {
      zlog_warn ("msg_read: OSPF API protocol version mismatch");
      return NULL;
    }

   
  bodylen = ntohs (hdr.msglen);
  if (bodylen > 0)
    {

       
      rlen = readn (fd, buf, bodylen);
      if (rlen < 0)
	{
	  zlog_warn ("msg_read: readn %s", safe_strerror (errno));
	  return NULL;
	}
      else if (rlen == 0)
	{
	  zlog_warn ("msg_read: Connection closed by peer");
	  return NULL;
	}
      else if (rlen != bodylen)
	{
	  zlog_warn ("msg_read: Cannot read message body!");
	  return NULL;
	}
    }

   
  msg = msg_new (hdr.msgtype, buf, ntohl (hdr.msgseq), ntohs (hdr.msglen));

  return msg;
}
