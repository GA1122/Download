 krb5_gss_process_context_token(minor_status, context_handle,
                                token_buffer)
     OM_uint32 *minor_status;
     gss_ctx_id_t context_handle;
     gss_buffer_t token_buffer;
 {
     krb5_gss_ctx_id_rec *ctx;
     OM_uint32 majerr;
  
      ctx = (krb5_gss_ctx_id_t) context_handle;
  
    if (! ctx->established) {
//     if (ctx->terminated || !ctx->established) {
          *minor_status = KG_CTX_INCOMPLETE;
          return(GSS_S_NO_CONTEXT);
      }
  
//      
//     if (ctx->proto) {
//         *minor_status = 0;
//         return(GSS_S_DEFECTIVE_TOKEN);
//     }
// 
       
  
      if (GSS_ERROR(majerr = kg_unseal(minor_status, context_handle,
                                      token_buffer,
                                      GSS_C_NO_BUFFER, NULL, NULL,
                                       KG_TOK_DEL_CTX)))
          return(majerr);
  
      
    return(krb5_gss_delete_sec_context(minor_status, &context_handle,
                                       GSS_C_NO_BUFFER));
//     ctx->terminated = 1;
//     return(GSS_S_COMPLETE);
  }