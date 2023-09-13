#include "window/window.h"

int main()
{
	window* wnd = new window();
	wnd->init();
	delete wnd;
	return 0;
}