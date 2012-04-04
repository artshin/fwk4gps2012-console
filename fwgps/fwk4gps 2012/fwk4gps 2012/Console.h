#include "Frame.h"      // for the Frame class definition
#include "Base.h"   
#include <vector>
#include <string>

class iAPIUserInput;
class iAPIText;
class iTexture;
class iObject;
class iTexture;
class iGraphic;
class iLight;
struct Rectf;
#define UP 1
#define DOWN 0

class Console : public iSwitch, public Base{
	static Console* address;   //points to this object


	// primitives for flags, iteration and so on ...
	//
	bool onScreen;					// flag that indicates whether the console is displayed
	bool animation;					// animation flag to start or finish animation
	bool readyToDrawText;
	int direction;
	int outputPos;
	int outputDisplaySize;
	unsigned outputMaxSize;
	unsigned lastInput;
	unsigned  lastToggle;			// time of the last toggle


	// Variables for manipulating text on the screen
	//
	std::wstring command;
	std::vector<std::wstring> output;

	iAPIUserInput* input;			// for the user input
	Rectf*    rect;					// box that holds coordinates for console position
	iAPIText* font;                 // points to the font at the API level
	iTexture* texture;				// for texturing

	//dynamic staff
	std::vector<iObject*>  dynamicObjects;          // points to objects
	std::vector<iTexture*> dynamicTexture;          // points to textures
	std::vector<iGraphic*> dynamicGraphic;          // points to graphics

	//private functions

	Console();
	Console(const Console& m); //prevents copying
	Console* operator=(const Console& m); //prevents assinment
	void doAnimation();
	void processCommand();                  //process the last command in output vector
	void trimOutput();
	~Console();

public:
	void sendMessage(wchar_t*);
	void render();
	
	
	void toogle();          
	void update();
	friend Console *createConsole();
	static Console* Address() { return address; } // static function that returns address of mc
	void Delete() const { delete this; address = 0; } 
	bool  isOn() const { return onScreen; }
	
};