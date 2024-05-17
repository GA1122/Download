void ResizeMoveCtrl(HWND hDlg, HWND hCtrl, int dx, int dy, int dw, int dh, float scale)
{
	RECT rect;
	POINT point;
	SIZE border;

	GetWindowRect(hCtrl, &rect);
	point.x = (right_to_left_mode && (hDlg != hCtrl))?rect.right:rect.left;
	point.y = rect.top;
	if (hDlg != hCtrl)
		ScreenToClient(hDlg, &point);
	GetClientRect(hCtrl, &rect);

	border = GetBorderSize(hCtrl);
	MoveWindow(hCtrl, point.x + (int)(scale*(float)dx), point.y + (int)(scale*(float)dy),
		(rect.right - rect.left) + (int)(scale*(float)dw + border.cx),
		(rect.bottom - rect.top) + (int)(scale*(float)dh + border.cy), TRUE);
	InvalidateRect(hCtrl, NULL, TRUE);
}