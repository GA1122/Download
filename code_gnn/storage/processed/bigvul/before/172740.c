static i32 ComparePictures(const void *ptr1, const void *ptr2)
{

 

 dpbPicture_t *pic1, *pic2;

 

    ASSERT(ptr1);
    ASSERT(ptr2);

    pic1 = (dpbPicture_t*)ptr1;
    pic2 = (dpbPicture_t*)ptr2;

  
 if (!IS_REFERENCE(*pic1) && !IS_REFERENCE(*pic2))
 {
 if (pic1->toBeDisplayed && !pic2->toBeDisplayed)
 return(-1);
 else if (!pic1->toBeDisplayed && pic2->toBeDisplayed)
 return(1);
 else
 return(0);
 }
  
 else if (!IS_REFERENCE(*pic2))
 return(-1);
  
 else if (!IS_REFERENCE(*pic1))
 return(1);
  
 else if (IS_SHORT_TERM(*pic1) && IS_SHORT_TERM(*pic2))
 {
 if (pic1->picNum > pic2->picNum)
 return(-1);
 else if (pic1->picNum < pic2->picNum)
 return(1);
 else
 return(0);
 }
  
 else if (IS_SHORT_TERM(*pic1))
 return(-1);
  
 else if (IS_SHORT_TERM(*pic2))
 return(1);
  
 else
 {
 if (pic1->picNum > pic2->picNum)
 return(1);
 else if (pic1->picNum < pic2->picNum)
 return(-1);
 else
 return(0);
 }
}
