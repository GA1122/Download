get_aix_arch( struct utsname *buf )
{
	char *ret = "UNK";
	char d[3];
	int model;

	 
	d[0] = buf->machine[ strlen( buf->machine ) - 4 ];
	d[1] = buf->machine[ strlen( buf->machine ) - 3 ];
	d[2] = '\0';
	model = strtol(d, NULL, 16);

	 
	switch(model)
	{
	  case 0x10:
		 
		break;

	  case 0x11:
	  case 0x14:
		 
		break;

	  case 0x18:
		 
		break;

	  case 0x1C:
		 
		break;

	  case 0x20:
		 
		break;

	  case 0x2E:
		 
		break;

	  case 0x30:
		 
		break;

	  case 0x31:
		 
		break;

	  case 0x34:
		 
		break;

	  case 0x35:
		 
		break;

	  case 0x37:
		 
		break;

	  case 0x38:
		 
		break;

	  case 0x41:
		 
		break;

	  case 0x42:
		 
		break;

	  case 0x43:
		 
		break;

	  case 0x45:
		 
		break;

	  case 0x46:
	  case 0x49:
		 
		break;

	  case 0x47:
		 
		break;

	  case 0x48:
		 
		break;

	  case 0x4C:
		 
		ret = strdup("PWR3II");
		break;

	  case 0x4D:
		 
		break;

	  case 0x57:
		 
		break;

	  case 0x58:
		 
		break;

	  case 0x59:
		 
		break;

	  case 0x5C:
		 
		break;

	  case 0x63:
		 
		break;

	  case 0x64:
		 
		break;

	  case 0x66:
		 
		break;

	  case 0x67:
		 
		break;

	  case 0x70:
		 
		break;

	  case 0x71:
		 
		break;

	  case 0x72:
		 
		break;

	  case 0x75:
		 
		break;

	  case 0x76:
		 
		break;

	  case 0x77:
		 
		break;

	  case 0x79:
		 
		break;

	  case 0x80:
		 
		break;

	  case 0x81:
		 
		break;

	  case 0x82:
		 
		break;

	  case 0x89:
		 
		break;

	  case 0x90:
		 
		break;

	  case 0x91:
		 
		break;

	  case 0x94:
		 
		break;

	  case 0xA0:
		 
		break;

	  case 0xA1:
		 
		break;

	  case 0xA3:
		 
		break;

	  case 0xA4:
		 
		break;

	  case 0xA6:
		 
		break;

	  case 0xA7:
		 
		break;

	  case 0xC4:
		 
		break;

	  default:
	  	 
		break;
	}

	return ret;
}
