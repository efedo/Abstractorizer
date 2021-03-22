#pragma once

#include <QWidget>
#include <QWindow>
#include <QVBoxLayout>
#include <iostream>

#include "engine_widget_internal.h"

class EngineWidget : public QWidget {
public:
	EngineWidgetInternal * internalPtr = 0;
	const std::string name;

	EngineWidget(QWidget* parent, EngineWidgetInternal* newinternalptr, const std::string newname) 
		: QWidget(parent), internalPtr(newinternalptr), name(newname) {
		std::cout << "Launching engine: " << name << "\n";
		QVBoxLayout* layout = new QVBoxLayout(this);
		this->setLayout(layout);
		layout->setMargin(0);
		layout->addWidget(internalPtr);
	}

	~EngineWidget() {
		std::cout << "Terminating engine: " << name << "\n";
	}

	virtual void exposeEvent(bool visible = true) {
		internalPtr->exposeEvent(visible);
		//QWidget::exposeEvent(visible);
	} // not a member of QWidget

	virtual void resizeEvent(QResizeEvent* resizeEvent) {
		internalPtr->resizeEvent(resizeEvent);
		QWidget::resizeEvent(resizeEvent);
	}

private:
	bool init = false;
};

// enginewidget -> magnumwidget -> magnumwidgetinternal;


// EngineWidget -> MagnumWidget
// EngineInternal -> MagnumInternal
//
//
//
//// Wraps an internal engine widget so that details/internal engine headers are not exposed
//template <class EngineWidgetInternalType>
//class EngineWidgetWrapper : public EngineWidget {
//	EngineWidgetWrapper(QWidget* parent, const std::string name) : EngineWidget(parent, name) {
//		internalPtr = std::make_unique<EngineWidgetInternalType>(this);
//	}
//
//	void exposeEvent(bool visible = true) override {
//		internalPtr->exposeEvent(visible);
//		EngineWidget::exposeEvent(visible);
//	};
//
//	void resizeEvent(QResizeEvent* resizeEvent) override { 
//		internalPtr->resizeEvent(resizeEvent);
//		EngineWidget::resizeEvent(resizeEvent);
//	}
//protected:
//	std::unique_ptr<EngineWidgetInternalType> internalPtr;
//};
//
////EngineWidget wtf2;