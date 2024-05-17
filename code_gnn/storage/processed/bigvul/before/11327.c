_gradient_walker_pixel (GradientWalker  *walker,
                        xFixed_32_32     x)
{
    int  dist, idist;
    CARD32  t1, t2, a, color;
    
    if (GRADIENT_WALKER_NEED_RESET (walker, x))
        _gradient_walker_reset (walker, x);
    
    dist  = ((int)(x - walker->left_x)*walker->stepper) >> 16;
    idist = 256 - dist;
    
     
    t1 = walker->left_rb*idist + walker->right_rb*dist;
    t1 = (t1 >> 8) & 0xff00ff;
    
    t2  = walker->left_ag*idist + walker->right_ag*dist;
    t2 &= 0xff00ff00;
    
    color = t2 & 0xff000000;
    a     = t2 >> 24;
    
    t1  = t1*a + 0x800080;
    t1  = (t1 + ((t1 >> 8) & 0xff00ff)) >> 8;
    
    t2  = (t2 >> 8)*a + 0x800080;
    t2  = (t2 + ((t2 >> 8) & 0xff00ff));
    
    return (color | (t1 & 0xff00ff) | (t2 & 0xff00));
}
