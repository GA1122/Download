 _gnutls_recv_handshake_header (gnutls_session_t session,
 			       gnutls_handshake_description_t type,
 			       gnutls_handshake_description_t * recv_type)
 {
   int ret;
   uint32_t length32 = 0;
   uint8_t *dataptr = NULL;	 
   size_t handshake_header_size = HANDSHAKE_HEADER_SIZE;
 
    
   if (session->internals.handshake_header_buffer.header_size ==
       handshake_header_size || (session->internals.v2_hello != 0
 				&& type == GNUTLS_HANDSHAKE_CLIENT_HELLO
 				&& session->internals.
 				handshake_header_buffer.packet_length > 0))
     {
  
        *recv_type = session->internals.handshake_header_buffer.recv_type;
  
//       if (*recv_type != type)
//        {
//          gnutls_assert ();
//          _gnutls_handshake_log
//            ("HSK[%x]: Handshake type mismatch (under attack?)\n", session);
//          return GNUTLS_E_UNEXPECTED_HANDSHAKE_PACKET;
//        }
// 
        return session->internals.handshake_header_buffer.packet_length;
      }
       ret =
 	_gnutls_handshake_io_recv_int (session, GNUTLS_HANDSHAKE,
 				       type, dataptr, SSL2_HEADERS);
 
       if (ret < 0)
 	{
 	  gnutls_assert ();
 	  return ret;
 	}
 
        
       if (ret != SSL2_HEADERS)
 	{
 	  gnutls_assert ();
 	  return GNUTLS_E_UNEXPECTED_PACKET_LENGTH;
 	}
       session->internals.handshake_header_buffer.header_size = SSL2_HEADERS;
     }