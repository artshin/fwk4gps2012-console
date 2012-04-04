#include "MessageCentre.h"
#include "iTexture.h"       // for the Texture Interface
#include "iCoordinator.h"   // for the Coordinator Interface
#include "ModellingLayer.h"
#include "iAPIText.h"       // for the APIText Interface
#include "Common_Symbols.h" // for Action and Sound enumerations
#include "APIPlatformSettings.h" // for API headers
#include "iAPIUserInput.h"
#include "Mappings.h"
#include "Translation.h"

#include <Windows.h>
#include <strsafe.h>

#define TEXT_ENTER_LATENCY 75

MessageCentre* MessageCentre::address = 0;


MessageCentre* createMessageCentre(){
	return MessageCentre::address ? MessageCentre::address : new MessageCentre();
}


void MessageCentre::toogle(){
	
	if (now - lastToggle > KEY_LATENCY) {
		lastToggle = now;
		
		readyToDrawText = false;
		animation = true;
		if(!onScreen) 
			onScreen = true;
	}
}


MessageCentre::MessageCentre(){
	address = this;

	onScreen = false;
	readyToDrawText = false;
	lastToggle = 0;
	lastInput = 0;
	animation = true;
	direction = UP;
	command[0] = '\0';
	
	input =  APIUserInputAddress();
	
	/*keyboard[0].usUsagePage = 1;
	keyboard[0].usUsage = 6;
	keyboard[0].dwFlags = RIDEV_NOLEGACY;
	keyboard[0].hwndTarget = NULL;


	*/
	output_maxSize = 50;
	/*
	if(RegisterRawInputDevices(keyboard, 1, sizeof(keyboard)) == FALSE){
		;
	}
	else
		output.push_back(L"Keyboard successfully registered");
		*/
	texture = CreateTexture(L"console.bmp");

	//addMessage(L"Console v1.0. To see the list of available commands enter help and press enter");
	output.push_back(L"Console v1.0. To see the list of available commands enter help and press enter");
	commands.push_back(L"");
	test.push_back(L"");
	//output.push_back(L"_");

	font = CreateAPIText(TEXT_TYPEFACE, TEXT_HEIGHT, TEXT_TOP, D3DCOLOR_ARGB(255,255,255,255));
	//CreateText(Rectf(0, 0.05f, 0.65f, 0.15f), nullptr, L" Camera: at ", position, 
    // Camera::getCurrent(), ' ', 1, 16, L"ARIAL", TEXT_CENTER);
	
	//command = CreateText(Rectf(0, 0.0f, 0.1f, 0.1f), nullptr,  L" Spinner x orientation ",
     // TEXT_HEIGHT, TEXT_TYPEFACE, TEXT_LEFT);

	rect = new Rectf(0.0f, 1.0f, 1.0f, 1.4f);
	
	//OutputDebugStringW(L"MessageCentre/: Message Centre successfully initialized\n");

	coordinator->add(this);
	
}


// if output is bigger than 50, then erase first element
void MessageCentre::trim_output(){
	if(output.size() > output_maxSize)
		output.erase(output.begin(), output.end()-output_maxSize);

}

void MessageCentre::do_animation(){
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


	/*
	MSG msg;
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_INPUT)
			

        /*if (msg.message == WM_QUIT) {
            rc = msg.wParam;
            keepgoing = false;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		retrievedMessage = true;
    }*/

void MessageCentre::addMessage(wchar_t* m){
	wchar_t temp[256];
	temp[0] = '\0';
	wcscpy_s(temp, m);

	output.push_back(temp);
}

void MessageCentre::render(){
	//onScreen = true;

	if(animation)
		do_animation();
		

	if(texture)
		texture->render(*rect, false, NOT_TRANSPARENT); //draw not transperent


	// regular height of the text string is 0.025f 
	if(readyToDrawText){
		trim_output();
		float draw_pos = 0.950f;
		font->draw(Rectf(0.005f, 0.975f, 1.0f, 1.0f), commands.back());
		for(int i = output.size()-1; i >= 0 && draw_pos > 0.650f; i--, draw_pos -= 0.025f ){ 
			font->draw(Rectf(0.005f, draw_pos, 1.0f, 1.0f), output[i]);

		}

	}
	//command->render();
	
	//OutputDebugStringW(L"MessageCentre/: is being rendered\n");
}

MessageCentre::~MessageCentre(){

}


void MessageCentre::update(){
	

	if(now - lastInput > TEXT_ENTER_LATENCY){
		lastInput = now;
		
		if(input->pressed(KEY_A)){
			test.back() += L"a";
			//wcscat_s(commands.back(),sizeof(commands.back()), L"a");
			//output.back() = command;
		}
		else if(input->pressed(KEY_B)){
			wcscat_s(command, L"b");
			//output.back() = command;
		}
		else if(input->pressed(KEY_C)){
			wcscat_s(command, L"c");
			//output.back() = command;
		}
		else if(input->pressed(KEY_D)){
			wcscat_s(command, L"d");
			//output.back() = command;
		}
		else if(input->pressed(KEY_E)){
			wcscat_s(command, L"e");
			//output.back() = command;
		}
		else if(input->pressed(KEY_F)){
			wcscat_s(command, L"f");
			//output.back() = command;
		}
		else if(input->pressed(KEY_G)){
			wcscat_s(command, L"g");
			//output.back() = command;
		}
		else if(input->pressed(KEY_H)){
			wcscat_s(command, L"h");
			//output.back() = command;
		}
		else if(input->pressed(KEY_I)){
			wcscat_s(command, L"i");
			//output.back() = command;
		}
		else if(input->pressed(KEY_J)){
			wcscat_s(command, L"j");
			//output.back() = command;
		}
		else if(input->pressed(KEY_K)){
			wcscat_s(command, L"k");
			//output.back() = command;
		}
		else if(input->pressed(KEY_L)){
			wcscat_s(command, L"l");
			//output.back() = command;
		}
		else if(input->pressed(KEY_M)){
			wcscat_s(command, L"m");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_N)){
			wcscat_s(command, L"n");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_O)){
			wcscat_s(command, L"o");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_P)){
			wcscat_s(command, L"p");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_Q)){
			wcscat_s(command, L"q");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_R)){
			wcscat_s(command, L"r");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_S)){
			wcscat_s(command, L"s");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_T)){
			wcscat_s(command, L"t");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_U)){
			wcscat_s(command, L"u");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_V)){
			wcscat_s(command, L"v");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_W)){
			wcscat_s(command, L"w");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_X)){
			wcscat_s(command, L"x");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_Y)){
			wcscat_s(command, L"y");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_Z)){
			wcscat_s(command, L"z");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_Z)){
			wcscat_s(command, L"z");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_1)){
			wcscat_s(command, L"1");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_2)){
			wcscat_s(command, L"2");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_3)){
			wcscat_s(command, L"3");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_4)){
			wcscat_s(command, L"4");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_5)){
			wcscat_s(command, L"5");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_6)){
			wcscat_s(command, L"6");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_7)){
			wcscat_s(command, L"7");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_8)){
			wcscat_s(command, L"8");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_9)){
			wcscat_s(command, L"9");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_0)){
			wcscat_s(command, L"0");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_SPACE)){
			wcscat_s(command, L" ");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_COMMA)){
			wcscat_s(command, L",");
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_BACKSPACE)){
			command[wcslen(command)-1] = '\0';
			//output.back() = command;() = command;
		}
		else if(input->pressed(KEY_ENTER)){
			test.push_back(L"test");
			output.push_back(test.back().c_str());
			//output.push_back(test[0].c_str());
			//wchar_t temp[256];
			//temp[0] = '\0';
			
			//output.push_back(commands.back());
			//commands.push_back(L"");
			
			//wmemcpy_s(output.back(),sizeof(output.back()),command, wcslen(command)*sizeof(wchar_t*));
			//wcscat_s(output.back(),command);
			//wchar_t temp[256];
			//temp[0] = '\0';
			//wcscat_s(temp,L"b");
			//wcscpy_s(temp, command);
			//wcscpy_s(temp,wcslen(command) *sizeof(wchar_t*),command);
			//output.push_back(temp);
			//output.push_back(L"_");
		}
	}

}