static void ShellSort(dpbPicture_t *pPic, u32 num)
{

    u32 i, j;
    u32 step;
 dpbPicture_t tmpPic;

    step = 7;

 while (step)
 {
 for (i = step; i < num; i++)
 {
            tmpPic = pPic[i];
            j = i;
 while (j >= step && ComparePictures(pPic + j - step, &tmpPic) > 0)
 {
                pPic[j] = pPic[j-step];
                j -= step;
 }
            pPic[j] = tmpPic;
 }
        step >>= 1;
 }

}
