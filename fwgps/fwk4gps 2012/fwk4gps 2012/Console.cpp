#include "Console.h"
#include "iTexture.h"      
#include "iCoordinator.h"  
#include "ModellingLayer.h"
#include "iAPIText.h"       // for the APIText Interface
#include "Common_Symbols.h" // for Action and Sound enumerations
#include "APIPlatformSettings.h" // for API headers
#include "iAPIUserInput.h"
#include "Translation.h"
#include "iGraphic.h"       
#include "iObject.h"       

#define TEXT_ENTER_LATENCY 85

Console* Console::address = 0;

Console* createConsole(){
	return Console::address ? Console::address : new Console();
}

//Constructor. It doesn't do any validation though

Console::Console(){
	address = this;

	onScreen = false;
	animation = true;
	readyToDrawText = false;
	direction = UP;
	outputPos = 0;
	outputDisplaySize = 12;
	outputMaxSize = 50;
	lastInput = 0;
	lastToggle = 0;
	
	output.push_back(L"Console v1.0. To see the list of available commands enter help and press enter");
	command += L"";
	
	input =  APIUserInputAddress();
	rect = new Rectf(0.0f, 1.0f, 1.0f, 1.4f);
	font = CreateAPIText(TEXT_TYPEFACE, TEXT_HEIGHT, TEXT_TOP, D3DCOLOR_ARGB(255,255,255,255));
	texture = CreateTexture(L"console.bmp");

	coordinator->add(this);
	
}

// Function for handling animation

void Console::doAnimation(){
	if(direction == UP){
		if(rect->topLeftY - 0.05f >= 0.6f)
			rect->topLeftY -= 0.05f;
		else{
			readyToDrawText = true;
			direction = DOWN;
			animation = false;
		}
	}
	else if (direction == DOWN){
		if(rect->topLeftY + 0.05f <= 1.0f)
			rect->topLeftY += 0.05f;
		else{
			direction = UP;
			onScreen = false;
			animation = false;
		}
	}
	
}

//this split function will only take one char to function correctly

std::vector<std::wstring> split(std::wstring s, const std::wstring delim){
	std::vector<std::wstring> result;

	if(s.empty())
		result.push_back(L"");
	else{
		if(delim.empty() || delim.size() > 1)
			result.push_back(s);
		else{
			for(unsigned i = 0, from = 0; i < s.size(); i++){
				if(s[i] == delim[0]){
					if(i != 0){
						result.push_back(s.substr(from, (i-from)));
						from = i+1;
					}
				}
				else if(i+1 == s.size()){
					if(s[i] != delim[0])
						result.push_back(s.substr(from, (i-from+1)));
					else
						result.push_back(s.substr(from, (i-from)));
				}
			}

		}
	}
	return result;
}

// processCommand will parse the input and display the result

void Console::processCommand(){
	// copy the string from output vector to 
	// a temp variable

	std::wstring tempCommand;

	if(!output.empty()){
		tempCommand += output.back();
		


		// The following if block takes care of 
		// breaking down the message into tokens 
		// and handling each token individually


		std::vector<std::wstring> tokens = split(tempCommand, L"_");
		
		// folowing if block is for handling 3 words command
		if(tokens.size() == 3){
			if(tokens[0] == L"create"){
				if(tokens[1] == L"box"){
					std::vector<std::wstring> values = split(tokens[2], L",");
					if(values.size() == 6){
						iGraphic* box   = CreateBox((float)_wtoi(values[0].c_str()),
													(float)_wtoi(values[1].c_str()),
													(float)_wtoi(values[2].c_str()) * MODEL_Z_AXIS, 
													(float)_wtoi(values[3].c_str()),
													(float)_wtoi(values[4].c_str()),
													(float)_wtoi(values[5].c_str()) * MODEL_Z_AXIS);

						dynamicGraphic.push_back(box);
						dynamicObjects.push_back(CreateObject(box));
					}
				}/*
				else if(tokens[1] == L"grid"){
					std::vector<std::wstring> values = split(tokens[2], L",");
					if(values.size() == 3){
						Colour blue(0, 0.3f, 0.9f);
						Reflectivity blueish = Reflectivity(Colour(0.9f, 0.9f, 0.9f));
						iGraphic* grid = CreateGrid((float)_wtoi(values[0].c_str()),
															(float)_wtoi(values[1].c_str()),
															(float)_wtoi(values[2].c_str())) ;
						dynamicGraphic.push_back(grid);
						dynamicObjects.push_back(CreateObject(grid,&blueish));
					}
				}*/
			}
			else if(tokens[0] == L"delete")
				if(tokens[1] == L"box"){
					unsigned value = _wtoi(tokens[2].c_str());
					if(value >= 0 && value < dynamicGraphic.size()){
						dynamicGraphic[value]->Delete();
						dynamicObjects[value]->Delete();
						dynamicGraphic.pop_back();
						dynamicObjects.pop_back();
					}

				}
		}
		else if(tokens.size() == 1){
			if(tokens[0] == L"help"){
				output.push_back(L"*********************************************************************************************************************");
				output.push_back(L"This version doesn't provide a lot of commands to work with, but new commands can be easily implemented ");
				output.push_back(L"in processCommand() function. Also, instead of spaces, underscores are put, it is designed this way. No hard feelings"); 
				output.push_back(L"So, at this point there are 3 working commands: add box, delete box and help");
				output.push_back(L"add box int,int,int,int,int,int - negative numbers also work");
				output.push_back(L"delete box int - So if you created one box only, to delete it you would type - delete box 0");
				output.push_back(L"help - just help");
				output.push_back(L"*********************************************************************************************************************");
			}
		}
	}
}


// If output is bigger than outputMaxSize(50), then erase first element
//
void Console::trimOutput(){
	if(output.size() > outputMaxSize)
		output.erase(output.begin(), output.end()-outputMaxSize);
}


Console::~Console(){
	input->Delete();
	texture->Delete();
	font->Delete();
}



//    *************************PUBLIC SECTION*********************


// send the message to console input

void Console::sendMessage(wchar_t* m){
	
	std::wstring inputString;
	inputString += m;

	if(inputString.size() > 0 && inputString.size() < 100){
		output.push_back(inputString);
		trimOutput();
		outputPos++;
	}
}

void Console::render(){

	if(animation)
		doAnimation();
		

	if(texture)
		texture->render(*rect, false, NOT_TRANSPARENT); //draw not transperent


	// regular height of the text string is 0.025f 
	if(readyToDrawText){
		trimOutput();
		float draw_pos = 0.950f;
		font->draw(Rectf(0.005f, 0.975f, 1.0f, 1.0f), command.c_str());
		for(int i = outputPos; i >= 0 && draw_pos > 0.650f; i--, draw_pos -= 0.025f ){ 
			font->draw(Rectf(0.005f, draw_pos, 1.0f, 1.0f), output[i].c_str());

		}

	}
}


void Console::toogle(){
	
	if (now - lastToggle > KEY_LATENCY) {
		lastToggle = now;
		
		readyToDrawText = false;
		animation = true;
		if(!onScreen) 
			onScreen = true;
	}
}


void Console::update(){
	

	if(now - lastInput > TEXT_ENTER_LATENCY){
		lastInput = now;
		
		if(input->pressed(KEY_A))			command += L"a";
		else if(input->pressed(KEY_B))		command += L"b";
		else if(input->pressed(KEY_C))		command += L"c";
		else if(input->pressed(KEY_D))		command += L"d";
		else if(input->pressed(KEY_E))		command += L"e";
		else if(input->pressed(KEY_F))		command += L"f";
		else if(input->pressed(KEY_G))		command += L"g";
		else if(input->pressed(KEY_H))		command += L"h";
		else if(input->pressed(KEY_I))		command += L"i";
		else if(input->pressed(KEY_J))		command += L"j";
		else if(input->pressed(KEY_K))		command += L"k";
		else if(input->pressed(KEY_L))		command += L"l";
		else if(input->pressed(KEY_M))		command += L"m";
		else if(input->pressed(KEY_N))		command += L"n";
		else if(input->pressed(KEY_O))		command += L"o";
		else if(input->pressed(KEY_P))		command += L"p";
		else if(input->pressed(KEY_Q))		command += L"q";
		else if(input->pressed(KEY_R))		command += L"r";
		else if(input->pressed(KEY_S))		command += L"s";
		else if(input->pressed(KEY_T))		command += L"t";
		else if(input->pressed(KEY_U))		command += L"u";
		else if(input->pressed(KEY_V))		command += L"v";
		else if(input->pressed(KEY_W))		command += L"w";
		else if(input->pressed(KEY_X))		command += L"x";
		else if(input->pressed(KEY_Y))		command += L"y";
		else if(input->pressed(KEY_Z))		command += L"z";
		else if(input->pressed(KEY_1))		command += L"1";
		else if(input->pressed(KEY_2))		command += L"2";
		else if(input->pressed(KEY_3))		command += L"3";
		else if(input->pressed(KEY_4))		command += L"4";
		else if(input->pressed(KEY_5))		command += L"5";
		else if(input->pressed(KEY_6))		command += L"6";
		else if(input->pressed(KEY_7))		command += L"7";
		else if(input->pressed(KEY_8))		command += L"8";
		else if(input->pressed(KEY_9))		command += L"9";
		else if(input->pressed(KEY_0))		command += L"0";
		else if(input->pressed(KEY_MINUS))	command += L"-";
		else if(input->pressed(KEY_SPACE))	command += L"_";
		else if(input->pressed(KEY_COMMA))	command += L",";
		else if(input->pressed(KEY_BACKSPACE)){
			if(command.size() > 0)
				command.erase(command.size()-1);
		}
		else if(input->pressed(KEY_ENTER)){
			if(command.size() > 0){
			output.push_back(command);
			processCommand();
			outputPos = output.size()-1;
			command.erase(0,command.size());
			}
		}
		else if(input->pressed(KEY_UP)){
			if(outputPos > outputDisplaySize)
				outputPos--;
		}
		else if(input->pressed(KEY_DOWN)){
			if(outputPos < (int)output.size()-1)
				outputPos++;
		}
	}

}