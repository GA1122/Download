 bool InlineFlowBox::nodeAtPoint(const HitTestRequest& request, HitTestResult& result, int x, int y, int tx, int ty)
 {
     IntRect overflowRect(visualOverflowRect());
     flipForWritingMode(overflowRect);
     overflowRect.move(tx, ty);
     if (!overflowRect.intersects(result.rectForPoint(x, y)))
         return false;
 
     for (InlineBox* curr = lastChild(); curr; curr = curr->prevOnLine()) {
         if ((curr->renderer()->isText() || !curr->boxModelObject()->hasSelfPaintingLayer()) && curr->nodeAtPoint(request, result, x, y, tx, ty)) {
             renderer()->updateHitTestResult(result, IntPoint(x - tx, y - ty));
             return true;
          }
      }
  
    FloatPoint boxOrigin = locationIncludingFlipping();
    boxOrigin.move(tx, ty);
    FloatRect rect(boxOrigin, IntSize(width(), height()));
//      
//     IntRect frameRect = roundedFrameRect();
//     int minX = frameRect.x();
//     int minY = frameRect.y();
//     int width = frameRect.width();
//     int height = frameRect.height();
// 
//      
//     bool noQuirksMode = renderer()->document()->inNoQuirksMode();
//     if (!hasTextChildren() && !noQuirksMode) {
//         RootInlineBox* rootBox = root();
//         int& top = isHorizontal() ? minY : minX;
//         int& logicalHeight = isHorizontal() ? height : width;
//         int bottom = min(rootBox->lineBottom(), top + logicalHeight);
//         top = max(rootBox->lineTop(), top);
//         logicalHeight = bottom - top;
//     }
//     
//      
//     IntRect rect(minX, minY, width, height);
//     flipForWritingMode(rect);
//     rect.move(tx, ty);
// 
      if (visibleToHitTesting() && rect.intersects(result.rectForPoint(x, y))) {
          renderer()->updateHitTestResult(result, flipForWritingMode(IntPoint(x - tx, y - ty)));  
          if (!result.addNodeToRectBasedTestResult(renderer()->node(), x, y, rect))
             return true;
     }
     
     return false;
 }