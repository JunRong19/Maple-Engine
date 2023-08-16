#ifndef GLHELPER_H
#define GLHELPER_H
struct GLHelper
	/*! GLHelper structure to encapsulate initialization stuff ...
	*/
{
	// Callbacks
	static void Error_Cb(int error, char const* description);
	static void Fbsize_Cb(GLFWwindow* ptr_win, int width, int height);
	// I/O callbacks
	static void Key_Cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void Mousebutton_Cb(GLFWwindow* pwin, int button, int action, int mod);
	static void Mousescroll_Cb(GLFWwindow* pwin, double xoffset, double yoffset);
	static void Mousepos_Cb(GLFWwindow* pwin, double xpos, double ypos);
	static void Print_Specs();
};

#endif /* GLHELPER_H */
