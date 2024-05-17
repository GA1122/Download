static void dumpRegion(String8& dump, const Region& region) {
 if (region.isEmpty()) {
        dump.append("<empty>");
 return;
 }

 bool first = true;
 Region::const_iterator cur = region.begin();
 Region::const_iterator const tail = region.end();
 while (cur != tail) {
 if (first) {
            first = false;
 } else {
            dump.append("|");
 }
        dump.appendFormat("[%d,%d][%d,%d]", cur->left, cur->top, cur->right, cur->bottom);
        cur++;
 }
}
