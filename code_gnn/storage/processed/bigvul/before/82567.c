void jswrap_graphics_drawRect(JsVar *parent, int x1, int y1, int x2, int y2) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
  graphicsDrawRect(&gfx, (short)x1,(short)y1,(short)x2,(short)y2);
  graphicsSetVar(&gfx);  
}