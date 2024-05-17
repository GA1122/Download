 static int lz4_uncompress(const char *source, char *dest, int osize)
 {
 	const BYTE *ip = (const BYTE *) source;
 	const BYTE *ref;
 	BYTE *op = (BYTE *) dest;
 	BYTE * const oend = op + osize;
 	BYTE *cpy;
 	unsigned token;
 	size_t length;
 	size_t dec32table[] = {0, 3, 2, 3, 0, 0, 0, 0};
 #if LZ4_ARCH64
 	size_t dec64table[] = {0, 0, 0, -1, 0, 1, 2, 3};
 #endif
 
 	while (1) {
 
 		 
 		token = *ip++;
 		length = (token >> ML_BITS);
 		if (length == RUN_MASK) {
 			size_t len;
 
  			len = *ip++;
  			for (; len == 255; length += 255)
  				len = *ip++;
// 			if (unlikely(length > (size_t)(length + len)))
// 				goto _output_error;
  			length += len;
  		}
  
 		 
 		cpy = op + length;
 		if (unlikely(cpy > oend - COPYLENGTH)) {
 			 
 			if (cpy != oend)
 				goto _output_error;
 
 			memcpy(op, ip, length);
 			ip += length;
 			break;  
 		}
 		LZ4_WILDCOPY(ip, op, cpy);
 		ip -= (op - cpy);
 		op = cpy;
 
 		 
 		LZ4_READ_LITTLEENDIAN_16(ref, cpy, ip);
 		ip += 2;
 
 		 
 		if (unlikely(ref < (BYTE *const) dest))
 			goto _output_error;
 
 		 
 		length = token & ML_MASK;
 		if (length == ML_MASK) {
 			for (; *ip == 255; length += 255)
 				ip++;
 			length += *ip++;
 		}
 
 		 
 		if (unlikely((op - ref) < STEPSIZE)) {
 #if LZ4_ARCH64
 			size_t dec64 = dec64table[op - ref];
 #else
 			const int dec64 = 0;
 #endif
 			op[0] = ref[0];
 			op[1] = ref[1];
 			op[2] = ref[2];
 			op[3] = ref[3];
 			op += 4;
 			ref += 4;
 			ref -= dec32table[op-ref];
 			PUT4(ref, op);
 			op += STEPSIZE - 4;
 			ref -= dec64;
 		} else {
 			LZ4_COPYSTEP(ref, op);
 		}
 		cpy = op + length - (STEPSIZE - 4);
 		if (cpy > (oend - COPYLENGTH)) {
 
 			 
 			if (cpy > oend)
 				goto _output_error;
 			LZ4_SECURECOPY(ref, op, (oend - COPYLENGTH));
 			while (op < cpy)
 				*op++ = *ref++;
 			op = cpy;
 			 
 			if (op == oend)
 				goto _output_error;
 			continue;
 		}
 		LZ4_SECURECOPY(ref, op, cpy);
 		op = cpy;  
 	}
 	 
 	return (int) (((char *)ip) - source);
 
 	 
 _output_error:
 	return (int) (-(((char *)ip) - source));
 }