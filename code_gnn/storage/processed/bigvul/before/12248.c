int tls1_ec_nid2curve_id(int nid)
	{
	 
	switch (nid)
		{
	case NID_sect163k1:  
		return 1;
	case NID_sect163r1:  
		return 2;
	case NID_sect163r2:  
		return 3;
	case NID_sect193r1:   
		return 4;
	case NID_sect193r2:   
		return 5;
	case NID_sect233k1:  
		return 6;
	case NID_sect233r1:   
		return 7;
	case NID_sect239k1:   
		return 8;
	case NID_sect283k1:  
		return 9;
	case NID_sect283r1:   
		return 10;
	case NID_sect409k1:   
		return 11;
	case NID_sect409r1:  
		return 12;
	case NID_sect571k1:   
		return 13;
	case NID_sect571r1:   
		return 14;
	case NID_secp160k1:  
		return 15;
	case NID_secp160r1:   
		return 16;
	case NID_secp160r2:   
		return 17;
	case NID_secp192k1:  
		return 18;
	case NID_X9_62_prime192v1:   
		return 19;
	case NID_secp224k1:   
		return 20;
	case NID_secp224r1:  
		return 21;
	case NID_secp256k1:   
		return 22;
	case NID_X9_62_prime256v1:   
		return 23;
	case NID_secp384r1:  
		return 24;
	case NID_secp521r1:   	
		return 25;
	default:
		return 0;
		}
	}
