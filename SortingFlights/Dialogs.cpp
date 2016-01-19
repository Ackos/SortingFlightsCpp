#include "Dialogs.h"


// Instantiating static maps of instances for each Dialog Type
std::map<std::string, QuestionDialog*> QuestionDialog::instances;
std::map<std::string, InputDialog*> InputDialog::instances;
std::map<int, InfoDialog*> InfoDialog::instances;

//----------------------------------------------------------------------------
// QuestionDialog Class Definition
//----------------------------------------------------------------------------

/*
	Constructor, private

	It copies message, set signal, and instantiate all graphic elements as its group members,
	also sets all needed graphical settings, such as collor, positions and so on.

	@param message -> (const char *) Message whici will be set as question.
	@param _signa -> (Fl_Callback *) pointer to a function which will be called before hidding.
*/
QuestionDialog::QuestionDialog(const char * message, Fl_Callback* _signal)
	: Fl_Window(WIDTH, HEIGHT, "Question")
{
	this -> _signal = _signal;
	strcpy_s(this -> message, message);
	color(Fl_Color(0x1A1A1A00));
	begin();
	question = new ListLabel(WIDTH / 2 - 40, 30, this -> message);
	yes = new CustomButton(WIDTH / 2 + 80, HEIGHT - 70, 120, 40, "Yes");
	no = new CustomButton(WIDTH  / 2 - 200, HEIGHT - 70, 120, 40, "No");
	yes -> callback((Fl_Callback *) &QuestionDialog::handleButton, (void *) this);
	yes -> shortcut(FL_Enter);
	no -> callback((Fl_Callback *) &QuestionDialog::handleButton, (void *) this);
	end();
	set_modal();
	// resizable(this);

	position(0, 0);

	status = EXIT;
}

/*
	Destructor, odd stuff when it's about FLTK.
*/
QuestionDialog::~QuestionDialog()
{
//	delete question;
//	delete yes;
//	delete no;
}

/*
	Singlethon pattern support. (Modified Singlethon).

	Method which checks map of instances if there is already instance with same message,
	it returns that, if not, it creates new one, and returns new one.

	@param title -> (const char *) message by which dialog is searched by.
	@param _signal -> (Fl_Callback *) (has default) if no instance found in map
						this will be passed to constructor, else it has no use.

	@returns Pointer to an instance of QuestionDialog with message sent as first parameter.
*/
QuestionDialog* QuestionDialog::getInstance(const char* title, Fl_Callback* _signal)
{
	std::string titleS(title);
	if (instances.find(title) == instances.end())
		instances[titleS] = new QuestionDialog(title, _signal);
	return instances[titleS];
}

/*
	Public interface for Fl_Window::show() method
*/
void QuestionDialog::showSelf()
{
	this -> show();
}

/*
	Handler method for buttons in dialog.

	Just changes status of dialog instance to appropriate status and calls its
	hide method, in order to close dialog.
*/
void QuestionDialog::handleButton(Fl_Widget* w, void* data)
{
	CustomButton* clicked =  (CustomButton *) w;
	if (clicked -> label() == "Yes")
		((QuestionDialog *) data) -> status = QuestionDialog::YES;
	else
		((QuestionDialog *) data) -> status = QuestionDialog::NO;

	((QuestionDialog *) data) -> hide();
}

/*
	Override Fl_Window::handle() -> event handler, to bypass some events.

	Bypass Escape and ALT+F4 just to first change status of dialog, and then
	sends signal back.

	@param _event -> (int) event code

	@returns Fl_Window::handle(_event)
*/
int QuestionDialog::handle(int _event)
{
	if (_event == FL_KEYDOWN)
	{
		int t = Fl::event_key();
		if (t == FL_Escape || t == FL_ALT + FL_F + 4)
			this -> status = QuestionDialog::EXIT;
	}

	if (_event == FL_CLOSE)
		this -> status = QuestionDialog::EXIT;

	return Fl_Window::handle(_event);
}

/*
	Override Fl_Window::hide() method in order to call Calback beffore hiding.
*/
void QuestionDialog::hide()
{
	if (_signal != nullptr)
		_signal((Fl_Widget *) this, (void *) &(this -> status));
	Fl_Window::hide();
}

//----------------------------------------------------------------------------
// END OF QuestionDialog Class Definition
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// InputDialog Class Definition
//----------------------------------------------------------------------------

/*
	Constructor, private

	It copies message, set signal, and instantiate all graphic elements as its group members,
	also sets all needed graphical settings, such as collor, positions and so on.

	@param message -> (const char *) message to be set as question for user.
	@param signal -> (Fl_Callback *) signal to be called before hidding dialog.
*/
InputDialog::InputDialog(const char * message, Fl_Callback* _signal)
	: Fl_Window(WIDTH, HEIGHT, "Question")
{
	this -> text[0] = '\0';
	this -> _signal = _signal;
	strcpy_s(this -> message, message);
	color(Fl_Color(0x1A1A1A00));
	begin();
	question = new ListLabel(WIDTH / 2 - 50, 30, this -> message);
	input = new Fl_Input(50, 70, 400, 30);
	input -> when(FL_WHEN_CHANGED);
	input -> callback(copyText, (void *) this);

	ok = new CustomButton(WIDTH / 2 + 80, HEIGHT - 70, 120, 40, "Ok");
	cancel = new CustomButton(WIDTH  / 2 - 200, HEIGHT - 70, 120, 40, "Cancel");
	ok -> callback(handleButton, (void *) this);
	ok -> shortcut(FL_Enter);
	cancel -> callback(handleButton, (void *) this);
	end();
	set_modal();
	// resizable(this);

	position(0, 0);

	status = CANCEL;
}

/*
	Destructor, odd stuff when it's about FLTK.
*/
InputDialog::~InputDialog()
{
//	delete question;
//	delete input;
//	delete ok;
//	delete cancel;
}

/*
	Singlethon pattern support. (Modified Singlethon).

	Method which checks map of instances if there is already instance with same message,
	it returns that, if not, it creates new one, and returns new one.

	@param title -> (const char *) message by which dialog is searched by.
	@param _signal -> (Fl_Callback *) (has default) if no instance found in map
						this will be passed to constructor, else it has no use.

	@returns Pointer to an instance of InputDialog with message sent as first parameter.

*/
InputDialog* InputDialog::getInstance(const char* title, Fl_Callback* _signal)
{
	std::string titleS(title);
	if (instances.find(title) == instances.end())
		instances[titleS] = new InputDialog(title, _signal);
	return instances[titleS];
}

/*
	Public interface for Fl_Window::show() method
*/
void InputDialog::showSelf()
{
	this -> input -> value("");
	this -> show();
}

/*
	Handler method for buttons in dialog.

	It changes status of this dialog and hides it, with one check if OK button is
	pressed it check if text is empty, then shows error message and doesn't call
	hide() method.
*/
void InputDialog::handleButton(Fl_Widget* w, void* data)
{
	CustomButton* clicked =  (CustomButton *) w;
	InputDialog* dialog = (InputDialog *) data;
	if (clicked -> label() == "Ok")
	{
		if (!(dialog -> text[0]))
		{
			InfoDialog::getInstance(InfoDialog::_ERROR, "You must enter something!");
			return;
		}
		((InputDialog *) data) -> status = InputDialog::OK;
	}
	else
		((InputDialog *) data) -> status = InputDialog::CANCEL;

	((InputDialog *) data) -> hide();
}

/*
	Handler method for input events.

	When ever input text changes copy text to text buffer.
*/
void InputDialog::copyText(Fl_Widget* w, void* userData)
{
	Fl_Input* input = (Fl_Input *) w;
	InputDialog* dialog = (InputDialog *) userData;

	strcpy_s(dialog -> text, input -> value());
}

/*
	Override Fl_Window::handle() -> event handler, to bypass some events.

	It bypass Escape and ALT_F4 keys in order to change status of dialog, and
	then returns controll to Fl_Window::handle() method

	@param _event -> event call.
	
	@returns Fl_Window::handle(_event);
*/
int InputDialog::handle(int _event)
{
	if (_event == FL_KEYDOWN)
	{
		int t = Fl::event_key();
		if (t == FL_Escape || t == FL_ALT + FL_F + 4)
			this -> status = InputDialog::CANCEL;
	}

	if (_event == FL_CLOSE)
		this -> status = InputDialog::CANCEL;

	return Fl_Window::handle(_event);
}

/*
	Override Fl_Window::hide() method in order to call Calback beffore hiding.
*/
void InputDialog::hide()
{
	if (_signal != nullptr)
		_signal((Fl_Widget *) this, (void *) &(this -> status));
	Fl_Window::hide();
}

//----------------------------------------------------------------------------
// END OF InputDialog Class Definition
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// InfoDialog Class Definition
//----------------------------------------------------------------------------

/*
	Constructor, private

	It copies message, set type and instantiate all graphic elements as its group members,
	also sets all needed graphical settings, such as collor, positions and so on.

	@param type -> (int) (enumeration) defines type of dialog which should be instantiated
	@param message -> (const char *) Message to be set as message for dialog.
*/
InfoDialog::InfoDialog(int type, const char * message)
	: Fl_Window(WIDTH, HEIGHT)
{
	messageText = new char[100];

	color(Fl_Color(0x1A1A1A00));
	begin();

	this -> message = new ListLabel(WIDTH / 2 - 50, 30, message);
	ok = new CustomButton(WIDTH / 2 + 80, HEIGHT - 70, 120, 40, "Ok");
	ok -> callback(handleButton, (void *) this);
	ok -> shortcut(FL_Enter);

	end();

	switch (type)
	{
	case INFO:
		label("Info");
		break;
	case _ERROR:
		label("Error");
		break;
	case SUCCESS:
		label("Success");
		break;
	}

	set_modal();
	// resizable(this);

	position(0, 0);
}

/*
	Destructor, odd stuff when it's about FLTK. (deallocates text buffer which is dynamically allocated).
*/
InfoDialog::~InfoDialog()
{
//	delete message;
//	delete ok;
	delete[] messageText;
}

/*
	Singlethon pattern support. (Modified Singlethon).

	Method which checks map of instances if there is already instance with same type,
	it sets its message and returns that, if not, it creates new one, sets its message,
	and returns new one.

	@param type -> (int) (enumeration) describes type of dialog.
	@param message -> (const char *) 
*/
InfoDialog* InfoDialog::getInstance(const int type, const char* message)
{
	if (instances.find(type) == instances.end())
		instances[type] = new InfoDialog(type, message);
	instances[type] -> setMessage(message);
	return instances[type];
}

/*
	Public interface for Fl_Window::show() method
*/
void InfoDialog::showSelf()
{
	this -> show();
}

/*
	Public interface, designed to store users message inside class buffer, so that user
	does not have to wory about lifetime of string which is passed as message.

	@param message -> (const char *) text to be copied into dialog label.
*/
void InfoDialog::setMessage(const char* message)
{
	for (int i = 0; i < 100; i++)
		messageText[i] = message[i];

	this -> message -> label(messageText);
}

/*
	Handler method for buttons in dialog. Just calls hide dialog.
*/
void InfoDialog::handleButton(Fl_Widget* w, void* data)
{
	((InfoDialog*) data) -> hide();
}