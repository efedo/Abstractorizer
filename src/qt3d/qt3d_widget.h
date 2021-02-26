#pragma once

#include "Abstractorizer/src/generic/engine_widget.h"
#include <iostream>

class Qt3DWindow;

class Qt3DWidget : public EngineWidget {
public:
	Qt3DWidget(QWidget* parent);
	~Qt3DWidget();
	void exposeEvent(bool visible = true) override;
	void resizeEvent(QResizeEvent*) override;
private:
	Qt3DWindow * windowPtr;
	QWidget * widgetPtr;
};

//class TestBlahClass {
//public:
//	TestBlahClass(int blah) {
//		int c = blah;
//	}
//	int c;
//};

//void testFuncB() {
//	TestBlahClass testClass(5);
//	std::cout << "Blah!";
//}