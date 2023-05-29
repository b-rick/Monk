#pragma once

#include <string>
#include "Widget.h"

class TextWidget : Widget
{
private:
	std::string content;

	const std::string header;
public:
	TextWidget(const char* header_text) : header{ std::string{header_text} }
	{
	}

	// Inherited via Widget
	virtual void setup() override;

	virtual void render() override;

	const std::string& get_text() const;

	const char* get_text_modifiable() const;
};
