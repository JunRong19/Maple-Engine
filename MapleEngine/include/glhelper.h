#ifndef GLHELPER_H
#define GLHELPER_H
struct GLHelper
	/*! GLHelper structure to encapsulate initialization stuff ...
	*/
{
	static bool Init(GLint w, GLint h, std::string t);
	static void Clean_Up();

	// Callbacks
	static void Error_Cb(int error, char const* description);
	static void Fbsize_Cb(GLFWwindow* ptr_win, int width, int height);
	// I/O callbacks
	static void Key_Cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void Mousebutton_Cb(GLFWwindow* pwin, int button, int action, int mod);
	static void Mousescroll_Cb(GLFWwindow* pwin, double xoffset, double yoffset);
	static void Mousepos_Cb(GLFWwindow* pwin, double xpos, double ypos);
	static void Print_Specs();

	static GLint width, height;
	static std::string title;
	static GLFWwindow* ptr_window;
};

#endif /* GLHELPER_H */
