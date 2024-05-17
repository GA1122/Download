t2p_sample_rgbaa_to_rgb(tdata_t data, uint32 samplecount)
{
	uint32 i;
	
     
    for(i = 0; i < 3 && i < samplecount; i++)
        memmove((uint8*)data + i * 3, (uint8*)data + i * 4, 3);
	for(; i < samplecount; i++)
		memcpy((uint8*)data + i * 3, (uint8*)data + i * 4, 3);

	return(i * 3);
}
