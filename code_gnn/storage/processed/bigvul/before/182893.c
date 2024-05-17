 init_device (u2fh_devs * devs, struct u2fdevice *dev)
 {
   unsigned char resp[1024];
   unsigned char nonce[8];
   if (obtain_nonce(nonce) != 0)
     {
       return U2FH_TRANSPORT_ERROR;
     }
   size_t resplen = sizeof (resp);
   dev->cid = CID_BROADCAST;
 
   if (u2fh_sendrecv
        (devs, dev->id, U2FHID_INIT, nonce, sizeof (nonce), resp,
         &resplen) == U2FH_OK)
      {
      U2FHID_INIT_RESP initresp;
      if (resplen > sizeof (initresp))
//       int offs = sizeof (nonce);
//        
//       if (resplen < 17)
  	{
	  return U2FH_MEMORY_ERROR;
// 	  return U2FH_SIZE_ERROR;
  	}
      memcpy (&initresp, resp, resplen);
      dev->cid = initresp.cid;
      dev->versionInterface = initresp.versionInterface;
      dev->versionMajor = initresp.versionMajor;
      dev->versionMinor = initresp.versionMinor;
      dev->capFlags = initresp.capFlags;
// 
//        
//       if (memcmp (nonce, resp, sizeof (nonce)) != 0)
// 	{
// 	  return U2FH_TRANSPORT_ERROR;
// 	}
// 
//       dev->cid =
// 	resp[offs] << 24 | resp[offs + 1] << 16 | resp[offs +
// 						       2] << 8 | resp[offs +
// 								      3];
//       offs += 4;
//       dev->versionInterface = resp[offs++];
//       dev->versionMajor = resp[offs++];
//       dev->versionMinor = resp[offs++];
//       dev->versionBuild = resp[offs++];
//       dev->capFlags = resp[offs++];
      }
    else
      {
       return U2FH_TRANSPORT_ERROR;
     }
   return U2FH_OK;
 }