FontFileSortDir(FontDirectoryPtr dir)
{
    FontFileSortTable (&dir->scalable);
    FontFileSortTable (&dir->nonScalable);
     
    FontFileSwitchStringsToBitmapPointers (dir);
}
