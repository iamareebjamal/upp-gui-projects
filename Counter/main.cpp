#include <CtrlLib/CtrlLib.h>
#include "draw_utils.cpp"

using namespace Upp;

struct MyAppWindow : TopWindow {
	int x, y;
	
	DrawUtils drawUtils;
	Counter ripple;
	
	bool pressed;
	Point prev_loc = Null;
	
	Vector<bool> clocks;
	Vector<bool> A;
	Vector<bool> B;
	Vector<bool> C;
	Vector<bool> D;
	
	void insert(Vector<bool>& pulses, bool item){
		if(pulses.GetCount()<26)
			pulses.Insert(0, item);
		else{
			pulses.Pop();
			pulses.Insert(0, item);
		}
	}
	
	void SetClocks(bool clock){
		ripple.next_cycle(clock);
		
		insert(clocks, clock);
		insert(A, ripple.a.get_data());
		insert(B, ripple.b.get_data());
		insert(C, ripple.c.get_data());
		insert(D, ripple.d.get_data());
	}
	
	void DrawTimingSignals(){
		drawUtils.DrawPulseTrain(650-20, clocks, drawUtils.MaterialGreen());
		drawUtils.DrawPulseTrain(720-20, A, 0x9C27B0);
		drawUtils.DrawPulseTrain(790-20, B, 0x33b5e5);
		drawUtils.DrawPulseTrain(860-20, C, 0x00E676);
		drawUtils.DrawPulseTrain(930-20, D, 0xf44336);
	}
	
	bool bounds(Point p){
		return p.x>=x-30 && p.x <=x+1350 && p.y>=y-55 && p.y<=y+420;	
	}
	
	virtual void LeftDown(Point pos, dword flags) {
		if(bounds(pos)){
			pressed = true;
			prev_loc = pos;
		}
		
        SetClocks(1);
        Refresh();
    }
    
    virtual void LeftUp(Point p, dword keyflags) {
        pressed = false;
        prev_loc = Null;
    }
        
    virtual void RightDown(Point pos, dword flags) {
        SetClocks(0);
        Refresh();
    }
    
    virtual void MouseMove(Point p, dword keyflags) {
        if(pressed){
            if(prev_loc!=Null){
                x += p.x-prev_loc.x;
                y += p.y - prev_loc.y;
            	Refresh();
            }
            prev_loc = p;
        }
    }
	
	virtual void Paint(Draw& w) {
		drawUtils = DrawUtils(&w);
		
		w.DrawRect(GetSize(), drawUtils.BG);
		
		drawUtils.DrawCounter(x,y, ripple);
		
		// Draw Timing Signals
		DrawTimingSignals();
	}

	MyAppWindow() {
		Title("BCD Ripple Counter");
		pressed = false;
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.x = 100;
	app.y = 200;
	app.SetRect(0, 0, 1500, 980);
	app.Run();
}
