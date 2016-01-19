#ifndef _DIALOGS_INCLUDED_
#define _DIALOGS_INCLUDED_

#include <map>
#include <string>

#include <FL\Fl_Window.H>
#include <FL\Fl.H>
#include <FL\Fl_Input.H>

#include "LabelsAndButtons.h"

//----------------------------------------------------------------------------
// QuestionDialog Class Declaration
//----------------------------------------------------------------------------

/*
	Class representing question dialog.

	It contains question with two buttons yes or no. It is designed to be
	modified Singlethon pattern (more alike Factory), as it contain static map
	of string-QuestionDialog * pairs. It creates instances unique by question sent
	to it.

	Also it contains Fl_Callback * which is called right before dialog is hidden.

	@author Acko.
*/
class QuestionDialog : Fl_Window
{
private:
	/*
		Map of all instances which are instantiated during runtime.
	*/
	static std::map<std::string, QuestionDialog*> instances;

	/*
		Pointer to an ListLabel object which will contain question for user.
	*/
	ListLabel* question;

	/*
		Pointer to an CustomButton object which present Yes button.
	*/
	CustomButton* yes;

	/*
		Pointer to an CustomButton object which present No button.
	*/	
	CustomButton* no;

	/*
		This callback will be called right before dialog is closed.

		It is set when creating dialog instance, and it is built to support signal travels through program.
	*/
	Fl_Callback* _signal;

	/*
		Char buffer which holds message for Question label (so that it doesn't go out of scope).
	*/
	char message[200];

	/*
		Constructor, private

		It copies message, set signal, and instantiate all graphic elements as its group members,
		also sets all needed graphical settings, such as collor, positions and so on.
	*/
	QuestionDialog(const char *, Fl_Callback *);
public:
	/*
		Enumeration representing states of Question dialog.
	*/
	enum { YES, NO, EXIT };

	/*
		Status of QuestionDialog instance (enumeration), changes when dialog is closed, so that we have information
		what user selected.
	*/
	int status;
	
	// self explained
	static const int WIDTH = 500;
	static const int HEIGHT = 150;

	/*
		Singlethon pattern support. (Modified Singlethon).

		Method which checks map of instances if there is already instance with same message,
		it returns that, if not, it creates new one, and returns new one.
	*/
	static QuestionDialog* getInstance(const char *, Fl_Callback * = nullptr);

	/*
		Destructor, odd stuff when it's about FLTK.
	*/
	~QuestionDialog();

	/*
		Public interface for Fl_Window::show() method
	*/
	void showSelf();

	/*
		Handler method for buttons in dialog.
	*/
	static void handleButton(Fl_Widget *, void *);

	/*
		Override Fl_Window::handle() -> event handler, to bypass some events.
	*/
	int handle(int);

	/*
		Override Fl_Window::hide() method in order to call Calback beffore hiding.
	*/
	void hide();
};

//----------------------------------------------------------------------------
// END OF QuestionDialog Class Declaration
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// InputDialog Class Declaration
//----------------------------------------------------------------------------


/*
	Class representing input dialog.

	It contains question with two buttons ok and cancell, and one input. It is designed to be
	modified Singlethon pattern (more alike Factory), as it contain static map
	of string-InputDialog * pairs. It creates instances unique by question sent
	to it.

	Also it contains Fl_Callback * which is called right before dialog is hidden.

	@author Acko.
*/
class InputDialog : Fl_Window
{
private:
	/*
		Map of all instances which are instantiated during runtime.
	*/
	static std::map<std::string, InputDialog*> instances;

	/*
		Pointer to an ListLabel object which will contain question for user.
	*/
	ListLabel* question;

	/*
		Pointer to an Fl_Input object which will take user input.
	*/
	Fl_Input* input;

	/*
		Pointer to an CustomButton object which present Ok button.
	*/
	CustomButton* ok;
	
	/*
		Pointer to an CustomButton object which present Cancel button.
	*/	
	CustomButton* cancel;

	/*
		This callback will be called right before dialog is closed.

		It is set when creating dialog instance, and it is built to support signal travels through program.
	*/
	Fl_Callback* _signal;

	/*
		Char buffer which holds message for Question label (so that it doesn't go out of scope).
	*/
	char message[200];

	/*
		Constructor, private

		It copies message, set signal, and instantiate all graphic elements as its group members,
		also sets all needed graphical settings, such as collor, positions and so on.
	*/
	InputDialog(const char *, Fl_Callback *);
public:
	/*
		Enumeration representing states of Input dialog.
	*/
	enum { OK, CANCEL };

	/*
		Status of InputDialog instance (enumeration), changes when dialog is closed, so that we have information
		what user selected.
	*/
	int status;

	/*
		Text buffer in which will be copied anything that user inputs in input, so that it will be
		easily accessible from outside class.
	*/
	char text[200];

	// self explained.
	static const int WIDTH = 500;
	static const int HEIGHT = 200;

	/*
		Singlethon pattern support. (Modified Singlethon).

		Method which checks map of instances if there is already instance with same message,
		it returns that, if not, it creates new one, and returns new one.
	*/
	static InputDialog* getInstance(const char *, Fl_Callback * = nullptr);

	/*
		Destructor, odd stuff when it's about FLTK.
	*/
	~InputDialog();

	/*
		Public interface for Fl_Window::show() method
	*/
	void showSelf();
	
	/*
		Handler method for buttons in dialog.
	*/
	static void handleButton(Fl_Widget *, void *);

	/*
		Handler method for input events (when ever input text changes copy text to text buffer).
	*/
	static void copyText(Fl_Widget *, void *);

	/*
		Override Fl_Window::handle() -> event handler, to bypass some events.
	*/
	int handle(int);

	/*
		Override Fl_Window::hide() method in order to call Calback beffore hiding.
	*/
	void hide();

};

//----------------------------------------------------------------------------
// END OF InputDialog Class Declaration
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// InfoDialog Class Declaration
//----------------------------------------------------------------------------

/*
	Class representing information dialog.

	It contains message with one button (ok). It is designed to be
	modified Singlethon pattern (more alike Factory), as it contain static map
	of int - InfoDialog * pairs. It creates instances unique by type sent
	to it.

	@author Acko.
*/
class InfoDialog : Fl_Window
{
private:
	/*
		Map of all instances which are instantiated during runtime.
	*/
	static std::map<int, InfoDialog *> instances;

	/*
		Pointer to an ListLabel object which will contain message for user.
	*/
	ListLabel* message;
	
	/*
		Pointer to an CustomButton object which present Ok button.
	*/
	CustomButton* ok;

	/*
		Dynamically allocated text buffer for message text, to copy user entered text, so that
		user can delete their text instances (label takes reference...)
	*/
	char* messageText;

	/*
		Constructor, private

		It copies message, set type and instantiate all graphic elements as its group members,
		also sets all needed graphical settings, such as collor, positions and so on.
	*/
	InfoDialog(const int, const char *);
public:
	/*
		Enumeration which describes type of InfoDialog. (It is used as key in map of all instances).
	*/
	enum { INFO, SUCCESS, _ERROR };

	// self explained.
	static const int WIDTH = 500;
	static const int HEIGHT = 150;

	/*
		Singlethon pattern support. (Modified Singlethon).

		Method which checks map of instances if there is already instance with same type,
		it sets its message and returns that, if not, it creates new one, sets its message, and returns new one.
	*/
	static InfoDialog* getInstance(const int, const char *);

	/*
		Destructor, odd stuff when it's about FLTK. (deallocates text buffer which is dynamically allocated).
	*/
	~InfoDialog();

	/*
		Public interface for Fl_Window::show() method
	*/
	void showSelf();

	/*
		Public interface, designed to store users message inside class buffer, so that user
		does not have to wory about lifetime of string which is passed as message.
	*/
	void setMessage(const char *);

	/*
		Handler method for buttons in dialog.
	*/
	static void handleButton(Fl_Widget *, void *);
};

#endif