

#include "Frame.h"      // for the Frame class definition
#include "Base.h"   
#include <vector>
#include <string>

class iAPIUserInput;
class iAPIText;
class iTexture;
struct Rectf;
#define UP 1
#define DOWN 0

class MessageCentre : public iSwitch, public Base{
	static MessageCentre* address;   //points to this object

	bool onScreen;					// flag that indicates whether the console is displayed
	bool animation;					// animation flag to start or finish animation
	bool readyToDrawText;
	unsigned  lastToggle;			// time of the last toggle
	int direction;
	unsigned lastInput;
	wchar_t command[255+1];

	
	unsigned int output_maxSize;
	std::vector<const wchar_t*> output;
	std::vector<wchar_t*> commands;

	std::vector<std::wstring> test;

	iAPIUserInput* input;
	Rectf*    rect;					// box that holds coordinates for console position
	iAPIText* font;                 // points to the font at the API level
	iTexture* texture;				// for texturing

	//std::vector<wchar_t*> text_lines;    

	MessageCentre();
	MessageCentre(const MessageCentre& m); //prevents copying
	MessageCentre* operator=(const MessageCentre& m); //prevents assinment
	void do_animation();
	void trim_output();
	~MessageCentre();

public:
	void addMessage(wchar_t*);
	void render();
	void update();
	bool  isOn() const { return onScreen; }
	void toogle();          
	friend MessageCentre *createMessageCentre();
	static MessageCentre* Address() { return address; } // static function that returns address of mc
	void Delete() const { delete this; address = 0; } 
};

//MessageCentre* getMessageCentre() { return MessageCentre::Address(); }
//bool consoleIsOn() { return MessageCentre::isOnScreen();}