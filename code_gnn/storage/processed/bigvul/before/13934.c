dump_icc_buffer(int buffersize, char filename[],byte *Buffer)
{
    char full_file_name[50];
    FILE *fid;

    gs_sprintf(full_file_name,"%d)%s_debug.icc",global_icc_index,filename);
    fid = gp_fopen(full_file_name,"wb");
    fwrite(Buffer,sizeof(unsigned char),buffersize,fid);
    fclose(fid);
}
