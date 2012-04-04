The list of changes made to framework to incorporate console


iCoordinator.h
	-added class Console;
	-added virtual void add(Console* c)	= 0;
	-added virtual void remove(Console* c) = 0;


Coordinator.h:
	-added void add(MessageCentre* m)
	-added MessageCentre* messageCentre to the list of privates
	-added void remove(MessageCentre* m)
	-added unsigned lastConsoleToggle;

Coordinator.cpp:
	Coordinator(void* hinst, int show):
		-added lastConsoleToggle = 0;
		-added messageCentre = createMessageCentre();
	render():
		-added render(MESSAGE_CENTRE);
	render(Category category):
		-added case MESSAGE_CENTRE...

ModellingLayer.h:
	-added MESSAGE_CENTRE into typedef enum Category
	-added define NOT_TRANSPARENT -1 for texture handling
	-added define TEXTURE_CLEAR '\xff' to render a clear texture

Mappings.h:
	-added KEY_APOSTROPHE to ACTION_KEY_MAP

Common_Symbols.h:
	-added MESSAGE_CENTRE_DISPLAY to enum Action
	-added L"Display Message Centre" to ACTION_DESCRIPTIONS

iAPITexture.h:
	-modified virtual void render(const Rectf&, unsigned char, bool = false)
	 to hold one more parameter.
	 Now it looks this way:
	 virtual void render(const Rectf&, unsigned char, bool = false, int key = -1) = 0;

APITexture.h:
	-updated void render(... int key = -1) to reflect changes made in interface

APITexture.cpp:
	-modified render(..., int key) function to make ColorKey parameter of 
	 D3DXCreateTextureFromFileEx function be more flexible.
	 The changes are: 
		if(key == -1)
		key = 0xFF000000;

		if (!tex) 
			setup((int)(width * (r.bottomRightX - r.topLeftX)), 
				  (int)(height * (r.bottomRightY - r.topLeftY)), 
				   key);
	 The code after is intact.

iTexture.h:
	-modified virtual void render(..., int transp = 1) = 0;
	 By introducing a new parameter with a default value,
	 this change doesn't screw up an entire framework.

Texture.h
	-updated render(..., int) declaration to reflect the change
	 described above.

Texture.cpp
	-modified the logic of render(..., int transp)
	 Now, depending on transp parameter it will call 
	 APITexture::render(...) with different parameters
	 that change the transperency of a texture.
	 The updated function looks this way now: 
	 
		void Texture::render(const Rectf& rect, bool back, int transp) {
	
			if (apiTexture){
				if(transp == -1)
					apiTexture->render(rect, TEXTURE_CLEAR, back, 0);
				else
					apiTexture->render(rect, TEXTURE_ALPHA, back);
			}

}


iAPIUserInput.h
	-added virtual bool  pressed(Key) const  = 0;

APIUserInput.h
	-added bool  pressed(Key) const;

APIUserInput.cpp
	-added
	bool APIUserInput::pressed(Key k) const{
	bool rc = false;
	
	if (keyboard){
		for (unsigned i = 0; !rc && i < nKeyboards; i++)
                rc = keyboard[i]->pressed(k);
	}

	return rc;
}

APIInputDevice.cpp
	-added to update() case DIK_BACKSPACE : k = KEY_BACKSPACE; break;


Design.h
	-added a console object to the lost of variables
	-modified design::update() to stop making changes to objects on the screen
	 when console is on