void res_clear_info(vorbis_info_residue *info){
 if(info){
 if(info->stagemasks)_ogg_free(info->stagemasks);
 if(info->stagebooks)_ogg_free(info->stagebooks);
    memset(info,0,sizeof(*info));
 }
}
