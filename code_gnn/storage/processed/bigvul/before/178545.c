 GahpServer::Reaper(Service *,int pid,int status)
 {
 	 
 
 	GahpServer *dead_server = NULL;
 	GahpServer *next_server = NULL;
 
 	GahpServersById.startIterations();
 	while ( GahpServersById.iterate( next_server ) != 0 ) {
 		if ( pid == next_server->m_gahp_pid ) {
 			dead_server = next_server;
 			break;
 		}
 	}
 
 	std::string buf;
 
 	sprintf( buf, "Gahp Server (pid=%d) ", pid );
 
 	if( WIFSIGNALED(status) ) {
 		sprintf_cat( buf, "died due to %s", 
 			daemonCore->GetExceptionString(status) );
 	} else {
 		sprintf_cat( buf, "exited with status %d", WEXITSTATUS(status) );
 	}
  
         if ( dead_server ) {
                 sprintf_cat( buf, " unexpectedly" );
               EXCEPT( buf.c_str() );
//                EXCEPT( "%s", buf.c_str() );
         } else {
                 sprintf_cat( buf, "\n" );
               dprintf( D_ALWAYS, buf.c_str() );
//                dprintf( D_ALWAYS, "%s", buf.c_str() );
         }
  }