#include <CtrlLib/CtrlLib.h>
#include "counter.h"
using namespace Upp;

class DrawUtils {
	private:
	
	Draw* w;

	Color bg;
	Color text;
	
	int text_size;
	bool clr;
	
	void SwitchColors(int q){
		if(q == 1)
			setColors(MaterialGreen());
		else
			setColors(MaterialRed());	
	}
	
	void DrawCenteredString(int x1, int y1, int x2, int y2, String string) {
		Rect rect(x1, y1, x2, y2);
		Font font = Arial(text_size);
		Point p = rect.CenterPos(GetTextSize(string.ToWString(), font));
		w->DrawText(p.x, p.y, string, font, text);
	}
	
	void DrawFF(int x, int y, JK& ff){
		w->DrawRect(x, y, 250,350, bg);
		w->DrawEllipse(x+125-15, y+350-5, 30, 30, BG, 5, bg);
		
		// J
		DrawCenteredString(x, y+50, x+50, y+50, (String) "J");
		// K
		DrawCenteredString(x, y+300, x+50, y+300, (String) "K");
		// Q
		DrawCenteredString(x+200, y+50, x+250, y+50, (String) "Q");
		// Q'
		DrawCenteredString(x+200, y+300, x+250, y+300, (String) "Q'");
		
		// Clock
		Vector<Point> triangle;
		triangle << Point(x, y+160) << Point(x, y+190) << Point(x+30, y+175);
		w->DrawPolygon(triangle, text, 10, text);
		
		
		// CLR
		text_size-=10;
		DrawCenteredString(x, y+320, x+250, y+320, (String) "CLR");
		
		
		/* FF based data */
		int q = ff.get_data();
		DrawCenteredString(x+55, y+50, x+55, y+50, AsString(ff.get_j()));
		DrawCenteredString(x+55, y+300, x+55, y+300, AsString(ff.get_k()));
		DrawCenteredString(x+195, y+50, x+195, y+50, AsString(q));
		DrawCenteredString(x+195, y+300, x+195, y+300, AsString(ff.get_data_bar()));
		DrawCenteredString(x+50, y+175, x+50, y+175, AsString(ff.get_clock()));
		
		text_size+=10;
		
		// Draw Q
		SwitchColors(q);
		DrawNode(x+220, y-50);
		w->DrawLine(x+250, y+50, x+300, y+50, 5, bg);
		w->DrawLine(x+300, y+50, x+300, y+172, 5, bg);
		
		// Draw Clear
		SwitchColors(clr);
		w->DrawLine(x+122, y+377, x+122, y+400, 5, bg);
		
		// Draw Clock
		SwitchColors(ff.get_clock());
		w->DrawLine(x-5, y+172, x-50, y+172, 5, bg);
	}
	
	void DrawNode(int x, int y) {
		w->DrawEllipse(x, y, 30, 30, bg, 5, bg);
	}
	
	public:
	
	DrawUtils() : text_size(35) {
		setColors(0x33b5e5);
	}
	
	DrawUtils(Draw* draw) : text_size(35) {
		w = draw;
		setColors(0x33b5e5);
	}
	
	void setDraw(Draw* draw){
		w = draw;
	}
	
	void setColors(int b) {
		setColors(getColor(b));	
	}
	
	void setColors(Color b) {
		bg = b;
		text = Blend(b, Black(), 150);	
	}
	
	
	
	void DrawCounter(int x, int y, Counter& counter){
		clr = counter.get_clear();
		
		setColors(0x9C27B0);
		DrawFF(x,y, counter.a);
		// Draw node on first clock
		DrawNode(x-50-15, y+172-15);
		
		setColors(0x33b5e5);
		DrawFF(x+=350,y, counter.b);
		
		setColors(0x00E676);
		DrawFF(x+=350,y, counter.c);
		
		setColors(0xf44336);
		DrawFF(x+=350,y, counter.d);
		
		// Mask last dangling node
		w->DrawLine(x+300, y+50, x+300, y+172, 5, BG);
		
		setColors(0x00E5FF);
		w->DrawEllipse(700, 10, 100, 100, bg, 5, bg);
		DrawCenteredString(700, 10, 800, 110, AsString(counter.get_value()));
		
		// Connect all clears
		SwitchColors(clr);
		w->DrawLine(x-1050+122, y+400, x+127, y+400, 5, bg);
		
	}
	
	void DrawPulseTrain(int y, Vector<bool>& pulses, int hexColor){
		DrawPulseTrain(y, pulses, getColor(hexColor));
	}
	
	void DrawPulseTrain(int y, Vector<bool>& pulses, Color color){
		int x = 100;
		setColors(color);
		
		w->DrawEllipse(x-75, y, 50, 50, bg, 5, bg);
		if(pulses.GetCount())
			DrawCenteredString(x-75, y, x-25, y+50, AsString((int)pulses[0]));
		
		for(int i = 0; i < pulses.GetCount(); i++){
			bool pulse = pulses[i];
			if(pulse){
				w->DrawRect(x, y, 50, 50, bg);
			} else {
				w->DrawRect(x, y+45, 50, 5, bg);
			}
			x+=50;
		}
	}
	
	static Color BG(){
		return getColor(0xF0F0F0);
	}
	
	static Color MaterialRed(){
		return getColor(0xf44336);
	}
	
	static Color MaterialGreen(){
		return getColor(0x64DD17);
	}
	
	static Color DARK_GRAY(){
		return getColor(0x3c3c3c);
	}
	
	
	static Color getColor(int hexcolor){
		int r = ( hexcolor >> 16 ) & 0xFF;
		int g = ( hexcolor >> 8 ) & 0xFF;
		int b = hexcolor & 0xFF;

		return Color(r, g, b);
	}
};