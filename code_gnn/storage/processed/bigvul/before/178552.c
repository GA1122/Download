 sysapi_translate_arch( const char *machine, const char *)
 {
 	char tmp[64];
 	char *tmparch;
 
 #if defined(AIX)
 	 
 	struct utsname buf;
 
 	if( uname(&buf) < 0 ) {
 		return NULL;
 	}
 
 	return( get_aix_arch( &buf ) );
 
 #elif defined(HPUX)
 
 	return( get_hpux_arch( ) );
 #else
 
 	if( !strcmp(machine, "alpha") ) {
 		sprintf( tmp, "ALPHA" );
 	}
 	else if( !strcmp(machine, "i86pc") ) {
 		sprintf( tmp, "INTEL" );
 	}
 	else if( !strcmp(machine, "i686") ) {
 		sprintf( tmp, "INTEL" );
 	}
 	else if( !strcmp(machine, "i586") ) {
 		sprintf( tmp, "INTEL" );
 	}
 	else if( !strcmp(machine, "i486") ) {
 		sprintf( tmp, "INTEL" );
 	}
 	else if( !strcmp(machine, "i386") ) {  
 #if defined(Darwin)
 		 
 		int ret;
 		char val[32];
 		size_t len = sizeof(val);
 
 		 
 		sprintf( tmp, "INTEL" );
 		ret = sysctlbyname("kern.osrelease", &val, &len, NULL, 0);
 		if (ret == 0 && strncmp(val, "10.", 3) == 0) {
 			 
 			sprintf( tmp, "X86_64" );
 		}
 #else
 		sprintf( tmp, "INTEL" );
 #endif
 	}
 	else if( !strcmp(machine, "ia64") ) {
 		sprintf( tmp, "IA64" );
 	}
 	else if( !strcmp(machine, "x86_64") ) {
 		sprintf( tmp, "X86_64" );
 	}
 	else if( !strcmp(machine, "amd64") ) {
 		sprintf( tmp, "X86_64" );
 	}
 	else if( !strcmp(machine, "sun4u") ) {
 		sprintf( tmp, "SUN4u" );
 	}
 	else if( !strcmp(machine, "sun4m") ) {
 		sprintf( tmp, "SUN4x" );
 	}
 	else if( !strcmp(machine, "sun4c") ) {
 		sprintf( tmp, "SUN4x" );
 	}
 	else if( !strcmp(machine, "sparc") ) {  
 		sprintf( tmp, "SUN4x" );
 	}
 	else if( !strcmp(machine, "Power Macintosh") ) {  
 		sprintf( tmp, "PPC" );
 	}
 	else if( !strcmp(machine, "ppc") ) {
 		sprintf( tmp, "PPC" );
 	}
 	else if( !strcmp(machine, "ppc32") ) {
 		sprintf( tmp, "PPC" );
 	}
 	else if( !strcmp(machine, "ppc64") ) {
 		sprintf( tmp, "PPC64" );
         }
         else {
               sprintf( tmp, machine );
//                sprintf( tmp, "%s", machine );
         }
  
         tmparch = strdup( tmp );
 	if( !tmparch ) {
 		EXCEPT( "Out of memory!" );
 	}
 	return( tmparch );
 #endif  
 }