#pragma once

#include <QWidget>
#include <iostream>

class EngineWidget : public QWidget {
public:
	const std::string name;

	EngineWidget(QWidget* parent, const std::string newname) : QWidget(parent), name(newname) {
		std::cout << "Launching engine: " << name << "\n";
	}

	~EngineWidget() {
		std::cout << "Terminating engine: " << name << "\n";
	}

	virtual void exposeEvent(bool visible = true) {} // not a member of QWidget
	virtual void resizeEvent(QResizeEvent* resizeEvent) override { QWidget::resizeEvent(resizeEvent); }
};

// Wraps an internal engine widget so that details/internal engine headers are not exposed
template <class EngineWidgetInternalType>
class EngineWidgetWrapper : public EngineWidget {
	EngineWidgetWrapper(QWidget* parent, const std::string name) : EngineWidget(parent, name) {
		internalPtr = std::make_unique<EngineWidgetInternalType>();
	}

	void exposeEvent(bool visible = true) override {
		internalPtr->exposeEvent(visible);
		EngineWidget::exposeEvent(visible);
	};

	void resizeEvent(QResizeEvent* resizeEvent) override { 
		internalPtr->resizeEvent(resizeEvent);
		EngineWidget::resizeEvent(resizeEvent);
	}
protected:
	std::unique_ptr<EngineWidgetInternalType> internalPtr;
};

