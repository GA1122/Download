static int convert_transform(i_ctx_t * i_ctx_p, ref *arr, ref *pproc)
{
    os_ptr op = osp;    
    int code;

     
    push(1);
     
    code = buildfunction(i_ctx_p, arr, pproc, 4);

    if (code < 0)
         
        code = buildfunction(i_ctx_p, arr, pproc, 0);

    return code;
}
