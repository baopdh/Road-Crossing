//#pragma once
//
//template <class T>
//class TrafficLightState {
//public:
//	virtual void goNext(SubVehicle<T> *, int) = 0;
//	virtual void selfDraw(int, int, ConsoleHandle *) = 0;
//};
//
//template <class T>
//class TrafficLightStateOn :public TrafficLightState<T> {
//public:
//	TrafficLightStateOn() {}
//	virtual ~TrafficLightStateOn() {}
//public:
//	void goNext(SubVehicle<T> *v, int tmp) {
//		v->setLightState(new TrafficLightStateOff<T>());
//		v->setSpeed(0);
//		delete this;
//	}
//
//	void selfDraw(int x, int y, ConsoleHandle *handle) {
//		handle->gotoXY(x, y);
//		handle->textcolor(ColorCode_Green);
//		cout << char(220);
//
//		handle->gotoXY(x, y + 1);
//		handle->textcolor(ColorCode_Grey);
//		cout << char(223);
//	}
//};
//
//template <class T>
//class TrafficLightStateOff :public TrafficLightState<T> {
//public:
//	TrafficLightStateOff() {}
//	virtual ~TrafficLightStateOff() {}
//public:
//	void goNext(SubVehicle<T> *v, int level) {
//		v->setLightState(new TrafficLightStateOn<T>());
//		
//		level %= 3;
//		if (!level)
//			level = 3;
//		v->setSpeed(level);
//		
//		delete this;
//	}
//
//	void selfDraw(int x, int y, ConsoleHandle *handle) {
//		handle->gotoXY(x, y);
//		handle->textcolor(ColorCode_Grey);
//		cout << char(220);
//
//		handle->gotoXY(x, y + 1);
//		handle->textcolor(ColorCode_Red);
//		cout << char(223);
//	}
//};