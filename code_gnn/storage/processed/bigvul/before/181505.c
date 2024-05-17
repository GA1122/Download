 static int dalvik_disassemble (RAsm *a, RAsmOp *op, const ut8 *buf, int len) {
 	int vA, vB, vC, payload = 0, i = (int) buf[0];
 	int size = dalvik_opcodes[i].len;
 	char str[1024], *strasm;
 	ut64 offset;
 	const char *flag_str; 
 
 	op->buf_asm[0] = 0;
 	if (buf[0] == 0x00) {  
 		switch (buf[1]) {
 		case 0x01:  
  			{
  				unsigned short array_size = buf[2] | (buf[3] << 8);
  				int first_key = buf[4] | (buf[5] << 8) | (buf[6] << 16) | (buf[7] << 24);
				sprintf (op->buf_asm, "packed-switch-payload %d, %d", array_size, first_key);
// 				snprintf (op->buf_asm, sizeof(op->buf_asm), "packed-switch-payload %d, %d", array_size, first_key);
  				size = 8;
  				payload = 2 * (array_size * 2);
  				len = 0;
 			}
 			break;
 		case 0x02:  
  			{
  				unsigned short array_size = buf[2] | (buf[3] << 8);
				sprintf (op->buf_asm, "sparse-switch-payload %d", array_size);
// 				snprintf (op->buf_asm, sizeof (op->buf_asm), "sparse-switch-payload %d", array_size);
  				size = 4;
  				payload = 2 * (array_size*4);
  				len = 0;
 			}
 			break;
 		case 0x03:  
 			if (len > 7) {
 				unsigned short elem_width = buf[2] | (buf[3] << 8);
 				unsigned int array_size = buf[4] | (buf[5] << 8) | (buf[6] << 16) | (buf[7] << 24);
 				snprintf (op->buf_asm, sizeof (op->buf_asm),
 					"fill-array-data-payload %d, %d",
 					elem_width, array_size);
 				payload = 2 * ((array_size * elem_width+1)/2);
 			}
 			size = 8;
 			len = 0;
 			break;
 		default:
 			 
 			break;
 		}
 	}
 	strasm = NULL;
 	if (size <= len) {
 		strncpy (op->buf_asm, dalvik_opcodes[i].name, sizeof (op->buf_asm) - 1);
 		strasm = strdup (op->buf_asm);
 		size = dalvik_opcodes[i].len;
 		switch (dalvik_opcodes[i].fmt) {
 		case fmtop: break;
  		case fmtopvAvB:
  			vA = buf[1] & 0x0f;
  			vB = (buf[1] & 0xf0) >> 4;
			sprintf (str, " v%i, v%i", vA, vB);
// 			snprintf (str, sizeof (str), " v%i, v%i", vA, vB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAAvBBBB:
  			vA = (int) buf[1];
  			vB = (buf[3] << 8) | buf[2];
			sprintf (str, " v%i, v%i", vA, vB);
// 			snprintf (str, sizeof (str), " v%i, v%i", vA, vB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAAAAvBBBB:  
  			vA = (buf[3] << 8) | buf[2];
  			vB = (buf[5] << 8) | buf[4];
			sprintf (str, " v%i, v%i", vA, vB);
// 			snprintf (str, sizeof (str), " v%i, v%i", vA, vB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAA:
  			vA = (int) buf[1];
			sprintf (str, " v%i", vA);
// 			snprintf (str, sizeof (str), " v%i", vA);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAcB:
  			vA = buf[1] & 0x0f;
  			vB = (buf[1] & 0xf0) >> 4;
			sprintf (str, " v%i, %#x", vA, vB);
// 			snprintf (str, sizeof (str), " v%i, %#x", vA, vB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAAcBBBB:
  			vA = (int) buf[1];
  			{
  				short sB = (buf[3] << 8) | buf[2];
				sprintf (str, " v%i, %#04hx", vA, sB);
// 				snprintf (str, sizeof (str), " v%i, %#04hx", vA, sB);
  				strasm = r_str_concat (strasm, str);
  			}
  			break;
 		case fmtopvAAcBBBBBBBB:
 			vA = (int) buf[1];
 			vB = buf[2] | (buf[3] << 8) | (buf[4] << 16) | (buf[5] << 24);
 			if (buf[0] == 0x17) {  
 				snprintf (str, sizeof (str), " v%i:v%i, 0x%08x", vA, vA + 1, vB);
 			} else {  
 				snprintf (str, sizeof (str), " v%i, 0x%08x", vA, vB);
 			}
 			strasm = r_str_concat (strasm, str);
 			break;
 		case fmtopvAAcBBBB0000:
 			vA = (int) buf[1];
 			vB = 0 | (buf[2] << 16) | (buf[3] << 24);
 			if (buf[0] == 0x19) {  
 				snprintf (str, sizeof (str), " v%i:v%i, 0x%08x", vA, vA + 1, vB);
 			} else {
 				snprintf (str, sizeof (str), " v%i, 0x%08x", vA, vB);
 			}
 			strasm = r_str_concat (strasm, str);
 			break;
 		case fmtopvAAcBBBBBBBBBBBBBBBB:
 			vA = (int) buf[1];
 			#define llint long long int
 			llint lB = (llint)buf[2] | ((llint)buf[3] << 8)|
 				((llint)buf[4] << 16) | ((llint)buf[5] << 24)|
  				((llint)buf[6] << 32) | ((llint)buf[7] << 40)|
  				((llint)buf[8] << 48) | ((llint)buf[9] << 56);
  			#undef llint
			sprintf (str, " v%i:v%i, 0x%"PFMT64x, vA, vA + 1, lB);
// 			snprintf (str, sizeof (str), " v%i:v%i, 0x%"PFMT64x, vA, vA + 1, lB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAAvBBvCC:
  			vA = (int) buf[1];
  			vB = (int) buf[2];
  			vC = (int) buf[3];
			sprintf (str, " v%i, v%i, v%i", vA, vB, vC);
// 			snprintf (str, sizeof (str), " v%i, v%i, v%i", vA, vB, vC);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAAvBBcCC:
  			vA = (int) buf[1];
  			vB = (int) buf[2];
  			vC = (int) buf[3];
			sprintf (str, " v%i, v%i, %#x", vA, vB, vC);
// 			snprintf (str, sizeof (str), " v%i, v%i, %#x", vA, vB, vC);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAvBcCCCC:
  			vA = buf[1] & 0x0f;
  			vB = (buf[1] & 0xf0) >> 4;
  			vC = (buf[3] << 8) | buf[2];
			sprintf (str, " v%i, v%i, %#x", vA, vB, vC);
// 			snprintf (str, sizeof (str), " v%i, v%i, %#x", vA, vB, vC);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtoppAA:
  			vA = (char) buf[1];
// 			 
  			snprintf (str, sizeof (str), " 0x%08"PFMT64x, a->pc + (vA * 2));  
  			strasm = r_str_concat (strasm, str);
  			break;
 		case fmtoppAAAA:
 			vA = (short) (buf[3] << 8 | buf[2]);
 			snprintf (str, sizeof (str), " 0x%08"PFMT64x, a->pc + (vA * 2));  
 			strasm = r_str_concat (strasm, str);
 			break;
  		case fmtopvAApBBBB:  
  			vA = (int) buf[1];
  			vB = (int) (buf[3] << 8 | buf[2]);
// 			 
  			snprintf (str, sizeof (str), " v%i, 0x%08"PFMT64x, vA, a->pc + (vB * 2));
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtoppAAAAAAAA:
  			vA = (int) (buf[2] | (buf[3] << 8) | (buf[4] << 16) | (buf[5] << 24));
// 			 
  			snprintf (str, sizeof (str), " 0x%08"PFMT64x, a->pc + (vA*2));  
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAvBpCCCC:  
  			vA = buf[1] & 0x0f;
  			vB = (buf[1] & 0xf0) >> 4;
  			vC = (int) (buf[3] << 8 | buf[2]);
// 			 
  			snprintf (str, sizeof (str)," v%i, v%i, 0x%08"PFMT64x, vA, vB, a->pc + (vC * 2));
  			strasm = r_str_concat (strasm, str);
  			break;
 		case fmtopvAApBBBBBBBB:
 			vA = (int) buf[1];
 			vB = (int) (buf[2] | (buf[3] << 8) | (buf[4] << 16) | (buf[5] << 24));
 			snprintf (str, sizeof (str), " v%i, 0x%08"PFMT64x, vA, a->pc + vB);  
 			strasm = r_str_concat (strasm, str);
 			break;
 		case fmtoptinlineI:
 			vA = (int) (buf[1] & 0x0f);
 			vB = (buf[3] << 8) | buf[2];
  			*str = 0;
  			switch (vA) {
  			case 1:
				sprintf (str, " {v%i}", buf[4] & 0x0f);
// 				snprintf (str, sizeof (str), " {v%i}", buf[4] & 0x0f);
  				break;
  			case 2:
				sprintf (str, " {v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4);
// 				snprintf (str, sizeof (str), " {v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4);
  				break;
  			case 3:
				sprintf (str, " {v%i, v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4, buf[5] & 0x0f);
// 				snprintf (str, sizeof (str), " {v%i, v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4, buf[5] & 0x0f);
  				break;
  			case 4:
				sprintf (str, " {v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
// 				snprintf (str, sizeof (str), " {v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
  						(buf[4] & 0xf0) >> 4, buf[5] & 0x0f, (buf[5] & 0xf0) >> 4);
  				break;
  			default:
				sprintf (str, " {}");
// 				snprintf (str, sizeof (str), " {}");
  			}
  			strasm = r_str_concat (strasm, str);
			sprintf (str, ", [%04x]", vB);
// 			snprintf (str, sizeof (str), ", [%04x]", vB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtoptinlineIR:
  		case fmtoptinvokeVSR:
  			vA = (int) buf[1];
  			vB = (buf[3] << 8) | buf[2];
  			vC = (buf[5] << 8) | buf[4];
			sprintf (str, " {v%i..v%i}, [%04x]", vC, vC + vA - 1, vB);
// 			snprintf (str, sizeof (str), " {v%i..v%i}, [%04x]", vC, vC + vA - 1, vB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtoptinvokeVS:
  			vA = (int) (buf[1] & 0xf0) >> 4;
  			vB = (buf[3] << 8) | buf[2];
  			switch (vA) {
  			case 1:
				sprintf (str, " {v%i}", buf[4] & 0x0f);
// 				snprintf (str, sizeof (str), " {v%i}", buf[4] & 0x0f);
  				break;
  			case 2:
				sprintf (str, " {v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4);
// 				snprintf (str, sizeof (str), " {v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4);
  				break;
  			case 3:
				sprintf (str, " {v%i, v%i, v%i}", buf[4] & 0x0f,
// 				snprintf (str, sizeof (str), " {v%i, v%i, v%i}", buf[4] & 0x0f,
  						(buf[4] & 0xf0) >> 4, buf[5] & 0x0f);
  				break;
  			case 4:
				sprintf (str, " {v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
// 				snprintf (str, sizeof (str), " {v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
  						(buf[4] & 0xf0) >> 4, buf[5] & 0x0f, (buf[5] & 0xf0) >> 4);
  				break;
  			default:
				sprintf (str, " {}");
// 				snprintf (str, sizeof (str), " {}");
  				break;
  			}
  			strasm = r_str_concat (strasm, str);
			sprintf (str, ", [%04x]", vB);
// 			snprintf (str, sizeof (str), ", [%04x]", vB);
  			strasm = r_str_concat (strasm, str);
  			break;
  		case fmtopvAAtBBBB:  
 			vA = (int) buf[1];
 			vB = (buf[3] << 8) | buf[2];
  			if (buf[0] == 0x1a) {
  				offset = R_ASM_GET_OFFSET (a, 's', vB);
  				if (offset == -1) {
					sprintf (str, " v%i, string+%i", vA, vB);
// 					snprintf (str, sizeof (str), " v%i, string+%i", vA, vB);
  				} else {
					sprintf (str, " v%i, 0x%"PFMT64x, vA, offset);
// 					snprintf (str, sizeof (str), " v%i, 0x%"PFMT64x, vA, offset);
  				}
  			} else if (buf[0] == 0x1c || buf[0] == 0x1f || buf[0] == 0x22) {
  				flag_str = R_ASM_GET_NAME (a, 'c', vB);
  				if (!flag_str) {
					sprintf (str, " v%i, class+%i", vA, vB);
// 					snprintf (str, sizeof (str), " v%i, class+%i", vA, vB);
  				} else {
					sprintf (str, " v%i, %s", vA, flag_str);
// 					snprintf (str, sizeof (str), " v%i, %s", vA, flag_str);
  				}
  			} else {
  				flag_str = R_ASM_GET_NAME (a, 'f', vB);
  				if (!flag_str) {
					sprintf (str, " v%i, field+%i", vA, vB);
// 					snprintf (str, sizeof (str), " v%i, field+%i", vA, vB);
  				} else {
					sprintf (str, " v%i, %s", vA, flag_str);
// 					snprintf (str, sizeof (str), " v%i, %s", vA, flag_str);
  				}
  			}
  			strasm = r_str_concat (strasm, str);
 			break;
 		case fmtoptopvAvBoCCCC:
 			vA = (buf[1] & 0x0f);
 			vB = (buf[1] & 0xf0) >> 4;
  			vC = (buf[3]<<8) | buf[2];
  			offset = R_ASM_GET_OFFSET (a, 'o', vC);
  			if (offset == -1) {
				sprintf (str, " v%i, v%i, [obj+%04x]", vA, vB, vC);
// 				snprintf (str, sizeof (str), " v%i, v%i, [obj+%04x]", vA, vB, vC);
  			} else {
				sprintf (str, " v%i, v%i, [0x%"PFMT64x"]", vA, vB, offset);
// 				snprintf (str, sizeof (str), " v%i, v%i, [0x%"PFMT64x"]", vA, vB, offset);
  			}
  			strasm = r_str_concat (strasm, str);
  			break;
 		case fmtopAAtBBBB:
 			vA = (int) buf[1];
  			vB = (buf[3] << 8) | buf[2];
  			offset = R_ASM_GET_OFFSET (a, 't', vB);
  			if (offset == -1) {
				sprintf (str, " v%i, thing+%i", vA, vB);
// 				snprintf (str, sizeof (str), " v%i, thing+%i", vA, vB);
  			} else {
				sprintf (str, " v%i, 0x%"PFMT64x, vA, offset);
// 				snprintf (str, sizeof (str), " v%i, 0x%"PFMT64x, vA, offset);
  			}
  			strasm = r_str_concat (strasm, str);
  			break;
 		case fmtopvAvBtCCCC:
 			vA = (buf[1] & 0x0f);
 			vB = (buf[1] & 0xf0) >> 4;
 			vC = (buf[3] << 8) | buf[2];
  			if (buf[0] == 0x20 || buf[0] == 0x23) {  
  				flag_str = R_ASM_GET_NAME (a, 'c', vC);
  				if (flag_str) {
					sprintf (str, " v%i, v%i, %s", vA, vB, flag_str);
// 					snprintf (str, sizeof (str), " v%i, v%i, %s", vA, vB, flag_str);
  				}
  				else {
					sprintf (str, " v%i, v%i, class+%i", vA, vB, vC);
// 					snprintf (str, sizeof (str), " v%i, v%i, class+%i", vA, vB, vC);
  				}
  			} else {
  				flag_str = R_ASM_GET_NAME (a, 'f', vC);
  				if (flag_str) {
					sprintf (str, " v%i, v%i, %s", vA, vB, flag_str);
// 					snprintf (str, sizeof (str), " v%i, v%i, %s", vA, vB, flag_str);
  				}
  				else {
					sprintf (str, " v%i, v%i, field+%i", vA, vB, vC);
// 					snprintf (str, sizeof (str), " v%i, v%i, field+%i", vA, vB, vC);
  				}
  			}
  			strasm = r_str_concat (strasm, str);
 			break;
 		case fmtopvAAtBBBBBBBB:
 			vA = (int) buf[1];
  			vB = (int) (buf[5] | (buf[4] << 8) | (buf[3] << 16) | (buf[2] << 24));
  			offset = R_ASM_GET_OFFSET (a, 's', vB);
  			if (offset == -1) {
				sprintf (str, " v%i, string+%i", vA, vB);
// 				snprintf (str, sizeof (str), " v%i, string+%i", vA, vB);
  			}
  			else {
				sprintf (str, " v%i, 0x%"PFMT64x, vA, offset);
// 				snprintf (str, sizeof (str), " v%i, 0x%"PFMT64x, vA, offset);
  			}
  			strasm = r_str_concat (strasm, str);
  			break;
 		case fmtopvCCCCmBBBB:
 			vA = (int) buf[1];
 			vB = (buf[3] << 8) | buf[2];
 			vC = (buf[5] << 8) | buf[4];
  			if (buf[0] == 0x25) {  
  				flag_str = R_ASM_GET_NAME (a, 'c', vB);
  				if (flag_str) {
					sprintf (str, " {v%i..v%i}, %s", vC, vC + vA - 1, flag_str);
// 					snprintf (str, sizeof (str), " {v%i..v%i}, %s", vC, vC + vA - 1, flag_str);
  				}
  				else {
					sprintf (str, " {v%i..v%i}, class+%i", vC, vC + vA - 1, vB);
// 					snprintf (str, sizeof (str), " {v%i..v%i}, class+%i", vC, vC + vA - 1, vB);
  				}
  			} else {
  				flag_str = R_ASM_GET_NAME (a, 'm', vB);
  				if (flag_str) {
					sprintf (str, " {v%i..v%i}, %s", vC, vC + vA - 1, flag_str);
// 					snprintf (str, sizeof (str), " {v%i..v%i}, %s", vC, vC + vA - 1, flag_str);
  				}
  				else {
					sprintf (str, " {v%i..v%i}, method+%i", vC, vC + vA - 1, vB);
// 					snprintf (str, sizeof (str), " {v%i..v%i}, method+%i", vC, vC + vA - 1, vB);
  				}
  			}
  			strasm = r_str_concat (strasm, str);
 			break;
 		case fmtopvXtBBBB:
 			vA = (int) (buf[1] & 0xf0) >> 4;
  			vB = (buf[3] << 8) | buf[2];
  			switch (vA) {
  				case 1:
					sprintf (str, " {v%i}", buf[4] & 0x0f);
// 					snprintf (str, sizeof (str), " {v%i}", buf[4] & 0x0f);
  					break;
  				case 2:
					sprintf (str, " {v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4);
// 					snprintf (str, sizeof (str), " {v%i, v%i}", buf[4] & 0x0f, (buf[4] & 0xf0) >> 4);
  					break;
  				case 3:
					sprintf (str, " {v%i, v%i, v%i}", buf[4] & 0x0f,
// 					snprintf (str, sizeof (str), " {v%i, v%i, v%i}", buf[4] & 0x0f,
  							(buf[4] & 0xf0) >> 4, buf[5] & 0x0f);
  					break;
  				case 4:
					sprintf (str, " {v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
// 					snprintf (str, sizeof (str), " {v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
  							(buf[4] & 0xf0) >> 4, buf[5] & 0x0f, (buf[5] & 0xf0) >> 4);
  					break;
  				case 5:
					sprintf (str, " {v%i, v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
// 					snprintf (str, sizeof (str), " {v%i, v%i, v%i, v%i, v%i}", buf[4] & 0x0f,
  							(buf[4] & 0xf0) >> 4, buf[5] & 0x0f, (buf[5] & 0xf0) >> 4, buf[1] & 0x0f);  
  					break;
  				default:
					sprintf (str, " {}");
// 					snprintf (str, sizeof (str), " {}");
  			}
  			strasm = r_str_concat (strasm, str);
  			if (buf[0] == 0x24) {  
  				flag_str = R_ASM_GET_NAME (a, 'c', vB);
  				if (flag_str) {
					sprintf (str, ", %s ; 0x%x", flag_str, vB);
// 					snprintf (str, sizeof (str), ", %s ; 0x%x", flag_str, vB);
  				} else {
					sprintf (str, ", class+%i", vB);
// 					snprintf (str, sizeof (str), ", class+%i", vB);
  				}
  			} else {
  				flag_str = R_ASM_GET_NAME (a, 'm', vB);
  				if (flag_str) {
					sprintf (str, ", %s ; 0x%x", flag_str, vB);
// 					snprintf (str, sizeof (str), ", %s ; 0x%x", flag_str, vB);
  				} else {
					sprintf (str, ", method+%i", vB);
// 					snprintf (str, sizeof (str), ", method+%i", vB);
  				}
  
  			}
 			strasm = r_str_concat (strasm, str);
 			break;
 		case fmtoptinvokeI:  
 		case fmtoptinvokeIR:
 		case fmt00:
 		default:
 			strcpy (op->buf_asm, "invalid ");
 			free (strasm);
 			strasm = NULL;
 			size = 2;
 		}
 		if (strasm) {
 			strncpy (op->buf_asm, strasm, sizeof (op->buf_asm) - 1);
 			op->buf_asm[sizeof (op->buf_asm) - 1] = 0;
 		} else {
 			strcpy (op->buf_asm , "invalid");
 		}
 	} else if (len > 0) {
 		strcpy (op->buf_asm, "invalid ");
 		op->size = len;
 		size = len;
 	}
 	op->payload = payload;
 	size += payload;  
 	op->size = size;
 	free (strasm);
 	return size;
 }