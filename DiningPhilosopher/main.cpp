#include <CtrlLib/CtrlLib.h>
#include <math.h>
#include <iostream>

using namespace Upp;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;

#define CLASSNAME MyAppWindow
#define PI 3.141592653589793

enum state { thinking, eating, hungry };

class Chopstick{
	private:
		int id;
		Mutex m;
		bool picked;
		
	public:
		Chopstick(int i):picked(false){
			id = i;
		}
		
		void pickup(){
			m.Enter();
			picked = true;
		}
		
		void putdown(){
			picked = false;
			m.Leave();
		}
		
		bool isPicked(){
			return picked;
		}
		
		int getID(){
			return id;
		}
};

class Philosopher{
	private:
		int id;
		state s;
		
	public:
		Chopstick* leftChopstick;
		Chopstick* rightChopstick;
		Philosopher(int i):s(thinking){
			id = i;
		}
		
		void setChopsticks(Chopstick* left, Chopstick* right){
			leftChopstick = left;
			rightChopstick = right;
		}
		
		int getID(){
			return id;
		}
		
		void setState(state st){
			s = st;
		}
		
		state getState(){
			return s;
		}
};


struct MyAppWindow : TopWindow {
	Draw* draw;
	EditInt philosophersText;
	Button run, stop;
	
	int numPhilosophers = 5;
	vector<Chopstick*> chopsticks;
	vector<Philosopher*> philosophers;
	
	// Thread shutdown
	bool done = false;
	vector<bool> running;
	
	virtual void Paint(Draw& w){
		draw = &w;
		
		createHalo(700, 800, 20, 5, 0xF48FB1, 0xF06292);
		createHalo(700, 830, 20, 5, 0x9575CD, 0x673AB7);
		createHalo(700, 860, 20, 5, 0xB39DDB, 0xB39DDB);
		w.DrawText(720, 790, "Eating", Arial(18), getColor(0x555555));
		w.DrawText(720, 820, "Hungry", Arial(18), getColor(0x555555));
		w.DrawText(720, 850, "Thinking", Arial(18), getColor(0x555555));
		
		int cx = GetSize().cx/2;
		int cy = GetSize().cy/2;
		int radius = 700;
		createHalo(cx, cy, radius);
		createHalo(cx, cy, 200, 15, 0xD1C4E9, 0xB39DDB);
		
		int n = numPhilosophers;
		for(int i=1; i<=n; i++){
			int angle = i*(360/n);
			int x = cx + (radius/2)*std::cos(angle*PI/180);
			int y = cy + (radius/2)*std::sin(angle*PI/180);
			// Chopsticks
			int phase_shift = 360/(2*n);
			int x1 = cx + (radius/2+50)*std::cos((angle-phase_shift)*PI/180);
			int y1 = cy + (radius/2+50)*std::sin((angle-phase_shift)*PI/180);
			int x2 = cx + (radius/2-270)*std::cos((angle-phase_shift)*PI/180);
			int y2 = cy + (radius/2-270)*std::sin((angle-phase_shift)*PI/180);
			
			createHalo(x, y, 100);
			
			w.DrawText(x-10, y-15, AsString(i), Arial(25), getColor(0x7E57C2));
			
			if(philosophers.size()>0){
				if(philosophers[i-1]->getState() == hungry){
					createHalo(x, y, 70, 5, 0x9575CD, 0x9575CD);
					w.DrawText(x-10, y-15, AsString(i), Arial(25), getColor(0xEDE7F6));
				} else if(philosophers[i-1]->getState() == eating){
					createHalo(x, y, 70, 5, 0xF06292, 0xF06292);
					w.DrawText(x-10, y-15, AsString(i), Arial(25), getColor(0xFCE4EC));
				}
			}
			
			w.DrawLine(x2, y2, x1, y1, 5, getColor(0x9575CD));
			
			if(chopsticks.size() > 0 && chopsticks[i-1]->isPicked())
				w.DrawLine(x2, y2, x1, y1, 7, getColor(0xF06292));
			
		}
	}
	
	void createHalo(int cx, int cy, int radius, int stroke=5, int colorIn=0xEDE7F6, int colorOut=0xD1C4E9){
		draw->DrawEllipse(cx-radius/2, cy-radius/2, radius, radius, getColor(colorIn), stroke, getColor(colorOut));
	}
	
	void execute(){
		numPhilosophers = philosophersText.GetData();
		chopsticks.resize(numPhilosophers);
		for (int i = 0; i < numPhilosophers; ++i){
			chopsticks[i] = new Chopstick(i+1);
		}
		
		philosophers.resize(numPhilosophers);
		for (int i = 0; i < numPhilosophers; ++i){
			philosophers[i] = new Philosopher(i+1);
			int left = i, right = (i+1)%numPhilosophers;
			if(left > right)
				left^=right^=left^=right;
			philosophers[i]->setChopsticks(chopsticks[left], chopsticks[right]);
		}

		for(bool threadStatus : running){
			if(threadStatus){
				PromptOK("Please wait for threads to shut down");
				return;
			}
		}
		done = false;
		
		vector<Thread> tasks(numPhilosophers);
		running.resize(numPhilosophers);
		for (int i = 0; i < numPhilosophers; ++i) {
			tasks[i].Run(THISBACK1(task, philosophers[i]));
		}
	}
	
	void stopThreads(){
		done = true;
	}
	
	void task(Philosopher* philosopher){
		while(true){
			if(done){ 
				running[philosopher->getID()-1] = false;
				
				philosopher->leftChopstick->putdown();
				philosopher->rightChopstick->putdown();
				philosopher->setState(thinking);
				Refresh();
				break;
			}
			
			running[philosopher->getID()-1] = true;
			cout << "\nPhilosopher " + to_string(philosopher->getID()) + " is thinking\n";
			philosopher->setState(thinking);
			Refresh();
			sleep(1);
	
			philosopher->leftChopstick->pickup();
			cout << "Philosopher " + to_string(philosopher->getID()) + " picked " + to_string(philosopher->leftChopstick->getID()) + " chopstick.\n";
			
			cout << "Philosopher " + to_string(philosopher->getID()) + " is hungry"<<endl;
			philosopher->setState(hungry);
			Refresh();
			philosopher->rightChopstick->pickup();
			cout << "Philosopher " + to_string(philosopher->getID()) + " picked " + to_string(philosopher->rightChopstick->getID()) + " chopstick.\n";
	
			cout << "Philosopher " + to_string(philosopher->getID()) + " is eating.\n\n";
			philosopher->setState(eating);
			Refresh();
			sleep(1);
			
			philosopher->leftChopstick->putdown();
			philosopher->rightChopstick->putdown();
		}
	}
	
	static Color getColor(int hexcolor){
		int r = ( hexcolor >> 16 ) & 0xFF;
		int g = ( hexcolor >> 8 ) & 0xFF;
		int b = hexcolor & 0xFF;

		return Color(r, g, b);
	}
	
	MyAppWindow(){
		Title("Dining Philosophers");
		*this << philosophersText.TopPosZ(8, 19).HCenterPos(100);
		*this << run.SetLabel("Run").TopPosZ(8, 19).RightPosZ(300, 50);
		*this << stop.SetLabel("Stop").TopPosZ(8, 19).RightPosZ(250, 50);
		philosophersText.SetData(numPhilosophers);
		
		run <<= THISBACK(execute);
		stop <<= THISBACK(stopThreads);
		philosophersText.WhenEnter = THISBACK(execute);
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.SetRect(0, 0, 900, 900);
	app.Run();
}
