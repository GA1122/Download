clamp_pattern_bbox(gs_pattern1_instance_t * pinst, gs_rect * pbbox,
                    int width, int height, const gs_matrix * pmat)
{
    double xstep = pinst->templat.XStep;
    double ystep = pinst->templat.YStep;
    double xmin = pbbox->q.x;
    double xmax = pbbox->p.x;
    double ymin = pbbox->q.y;
    double ymax = pbbox->p.y;
    int ixpat, iypat, iystart;
    double xpat, ypat;
    double xlower, xupper, ylower, yupper;
    double xdev, ydev;
    gs_rect dev_page, pat_page;
    gs_point dev_pat_origin, dev_step;
    int code;

    double xepsilon = FLT_EPSILON * width;
    double yepsilon = FLT_EPSILON * height;

     
     
    if (xstep < 0)
        xstep *= -1;
    if (ystep < 0)
        ystep *= -1;
     
    dev_page.p.x = dev_page.p.y = 0;
    dev_page.q.x = width;
    dev_page.q.y = height;
    code = gs_bbox_transform_inverse(&dev_page, pmat, &pat_page);
    if (code < 0)
        return code;
     
    gs_point_transform(0.0, 0.0, pmat, &dev_pat_origin);
     
    ixpat = (int) floor((pat_page.p.x - pinst->templat.BBox.q.x) / xstep);
    iystart = (int) floor((pat_page.p.y - pinst->templat.BBox.q.y) / ystep);

     
    for (; ; ixpat++) {
        xpat = ixpat * xstep;
        for (iypat = iystart; ; iypat++) {
            ypat = iypat * ystep;
             
            gs_point_transform(xpat, ypat, pmat, &dev_step);
            xdev = dev_step.x - dev_pat_origin.x;
            ydev = dev_step.y - dev_pat_origin.y;
             
            xlower = (xdev + pbbox->p.x > 0) ? pbbox->p.x : -xdev;
            xupper = (xdev + pbbox->q.x < width) ? pbbox->q.x : -xdev + width;
            ylower = (ydev + pbbox->p.y > 0) ? pbbox->p.y : -ydev;
            yupper = (ydev + pbbox->q.y < height) ? pbbox->q.y : -ydev + height;

             
            if (xlower + xepsilon < xupper && ylower + yepsilon < yupper) {
                 
                if (xlower < xmin)
                    xmin = xlower;
                if (xupper > xmax)
                    xmax = xupper;
                if (ylower < ymin)
                    ymin = ylower;
                if (yupper > ymax)
                    ymax = yupper;
            }
            if (ypat > pat_page.q.y - pinst->templat.BBox.p.y)
                break;
        }
        if (xpat > pat_page.q.x - pinst->templat.BBox.p.x)
            break;
    }
     
    if (xmin < xmax && ymin < ymax) {
        pbbox->p.x = xmin;
        pbbox->q.x = xmax;
        pbbox->p.y = ymin;
        pbbox->q.y = ymax;
    } else {
         
        pbbox->p.x = pbbox->p.y = 0;
        pbbox->q.x = pbbox->q.y = 1;
    }
    return 0;
}
