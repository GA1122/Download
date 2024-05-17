 int EVP_DecodeUpdate(EVP_ENCODE_CTX *ctx, unsigned char *out, int *outl,
 	     const unsigned char *in, int inl)
 	{
 	int seof= -1,eof=0,rv= -1,ret=0,i,v,tmp,n,ln,exp_nl;
 	unsigned char *d;
 
 	n=ctx->num;
 	d=ctx->enc_data;
 	ln=ctx->line_num;
 	exp_nl=ctx->expect_nl;
 
 	 
 	if ((inl == 0) || ((n == 0) && (conv_ascii2bin(in[0]) == B64_EOF)))
 		{ rv=0; goto end; }
 		
 	 
 	for (i=0; i<inl; i++)
 		{
 		 
 		if (ln >= 80) { rv= -1; goto end; }
 
 		 
 		tmp= *(in++);
 		v=conv_ascii2bin(tmp);
 		 
 		if (!B64_NOT_BASE64(v))
 			{
 			OPENSSL_assert(n < (int)sizeof(ctx->enc_data));
 			d[n++]=tmp;
 			ln++;
 			}
 		else if (v == B64_ERROR)
 			{
 			rv= -1;
 			goto end;
 			}
 
 		 
 		if (tmp == '=')
 			{
 			if (seof == -1) seof=n;
 			eof++;
 			}
 
 		if (v == B64_CR)
 			{
 			ln = 0;
 			if (exp_nl)
 				continue;
 			}
 
 		 
 		if (v == B64_EOLN)
 			{
 			ln=0;
 			if (exp_nl)
 				{
 				exp_nl=0;
 				continue;
 				}
 			}
 		exp_nl=0;
 
 		 
 		if (((i+1) == inl) && (((n&3) == 0) || eof))
 			{
 			v=B64_EOF;
 			 
 			eof = 0;
 			if (d[n-1] == '=') eof++;
 			if (d[n-2] == '=') eof++;
 			 
 			}
 
 		if ((v == B64_EOF && (n&3) == 0) || (n >= 64))
 			{
 			 
 			if ((v != B64_EOF) && (n >= 64)) exp_nl=1;
 			if (n > 0)
 				{
                                 v=EVP_DecodeBlock(out,d,n);
                                 n=0;
                                 if (v < 0) { rv=0; goto end; }
//                                if (eof > v) { rv=-1; goto end; }
                                 ret+=(v-eof);
                                 }
                         else
 				eof=1;
 				v=0;
 				}
 
 			 
 			if ((v < ctx->length) && eof)
 				{
 				rv=0;
 				goto end;
 				}
 			else
 				ctx->length=v;
 
 			if (seof >= 0) { rv=0; goto end; }
 			out+=v;
 			}
 		}