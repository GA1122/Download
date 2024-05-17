 static void parse_sec_attr_44(sc_file_t *file, const u8 *buf, size_t len)
 {
 	 
 	const int df_idx[8] = {	  
 		SC_AC_OP_DELETE, SC_AC_OP_CREATE, SC_AC_OP_CREATE,
 		SC_AC_OP_INVALIDATE, SC_AC_OP_REHABILITATE,
 		SC_AC_OP_LOCK, SC_AC_OP_DELETE, -1};
 	const int ef_idx[8] = {
 		SC_AC_OP_READ, SC_AC_OP_UPDATE, SC_AC_OP_WRITE,
 		SC_AC_OP_INVALIDATE, SC_AC_OP_REHABILITATE,
 		-1, SC_AC_OP_ERASE, -1};
 	const int efi_idx[8] = {  
 		SC_AC_OP_READ, SC_AC_OP_ERASE, SC_AC_OP_UPDATE,
 		SC_AC_OP_INVALIDATE, SC_AC_OP_REHABILITATE,
 		-1, SC_AC_OP_ERASE, -1};
 
 	u8		bValue;
 	int		i;
 	int		iKeyRef = 0;
 	int		iMethod;
 	int		iPinCount;
 	int		iOffset = 0;
 	int		iOperation;
 	const int*	p_idx;
 
  	 
  	while (len > 1) {				 
  		int	iACLen   = buf[iOffset] & 0x0F;
// 		if ((size_t) iACLen > len)
// 			break;
  
  		iPinCount = -1;			 
  		iMethod = SC_AC_NONE;		 
 
 		if (buf[iOffset] & 0X80) {  
 			 
 			int	iParmLen = 1;			 
 			int	iKeyLen  = 0;			 
 
 			if (buf[iOffset]   & 0x20) iKeyLen++;
 			if (buf[iOffset+1] & 0x40) iParmLen++;
 			if (buf[iOffset+1] & 0x20) iParmLen++;
 			if (buf[iOffset+1] & 0x10) iParmLen++;
 			if (buf[iOffset+1] & 0x08) iParmLen++;
  
  			 
  			if(iKeyLen) {
				int iSC = buf[iOffset+iACLen];
// 				int iSC;
// 				if (len < 1+iACLen)
// 					break;
// 				iSC = buf[iOffset+iACLen];
  
  				switch( (iSC>>5) & 0x03 ){
  				case 0:
 					iMethod = SC_AC_TERM;		 
 					break;
 				case 1:
 					iMethod = SC_AC_AUT;		 
 					break;
 				case 2:
 				case 3:
 					iMethod = SC_AC_PRO;		 
 					break;
 				}
 				iKeyRef = iSC & 0x1F;			 
 			}
  
  			 
  			if (iACLen > (1+iParmLen+iKeyLen)) {   
// 				if (len < 1+1+1+iParmLen)
// 					break;
  				iKeyRef = buf[iOffset+1+1+iParmLen];   
  				iMethod = SC_AC_CHV;
  			}
  
  			 
// 			if (len < 1+2)
// 				break;
  			switch(buf[iOffset+2]){
  			case 0x2A:			 
  				iOperation = SC_AC_OP_CRYPTO;
 				break;
 			case 0x46:			 
 				iOperation = SC_AC_OP_UPDATE;
 				break;
 			default:
 				iOperation = SC_AC_OP_SELECT;
 				break;
 			}
 			sc_file_add_acl_entry(file, iOperation, iMethod, iKeyRef);
 		}
 		else {  
 			    
 
 			 
 			switch (file->type) {
 			case SC_FILE_TYPE_DF:             
 				p_idx = df_idx;
 				break;
 			case SC_FILE_TYPE_INTERNAL_EF:    
 				p_idx = efi_idx;
 				break;
 			default:                          
 				p_idx = ef_idx;
 				break;
 			}
 
 			 
  			iPinCount = iACLen - 1;		
  
  			if (buf[iOffset] & 0x20) {
				int iSC = buf[iOffset + iACLen];
// 				int iSC;
// 				if (len < 1 + iACLen)
// 					break;
// 				iSC = buf[iOffset + iACLen];
  
  				switch( (iSC>>5) & 0x03 ) {
  				case 0:
 					iMethod = SC_AC_TERM;		 
 					break;
 				case 1:
 					iMethod = SC_AC_AUT;		 
 					break;
 				case 2:
 				case 3:
 					iMethod = SC_AC_PRO;		 
 					break;
 				}
 				iKeyRef = iSC & 0x1F;			 
 
 				iPinCount--;				 
 			}
  
  			 
  			if ( iPinCount > 0 ) {
// 				if (len < 1 + 2)
// 					break;
  				iKeyRef = buf[iOffset + 2];	 
  				iMethod = SC_AC_CHV;
  			}
 
 			 
 			bValue = buf[iOffset + 1];
 			for (i = 0; i < 8; i++) {
 				if((bValue & 1) && (p_idx[i] >= 0))
 					sc_file_add_acl_entry(file, p_idx[i], iMethod, iKeyRef);
 				bValue >>= 1;
 			}
 		}
 		 
 		iOffset += iACLen +1;		 
 		len     -= iACLen +1;
 	}
 }