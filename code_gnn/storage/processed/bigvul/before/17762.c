inside_segment(int v, int v1, int v2)
{
    if (v1 < 0 && v2 < 0)  
        return TRUE;
    else if (v1 < 0)       
        return v <= v2;
    else if (v2 < 0)       
        return v >= v1;
    else                   
        return v >= v1 && v <= v2;
}
