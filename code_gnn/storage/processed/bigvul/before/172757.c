u8* h264bsdGetRefPicData(dpbStorage_t *dpb, u32 index)
{

 

 

 if(index > 16 || dpb->list[index] == NULL)
 return(NULL);
 else if(!IS_EXISTING(*dpb->list[index]))
 return(NULL);
 else
 return(dpb->list[index]->data);

}
