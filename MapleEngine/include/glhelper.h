#ifndef GLHELPER_H
#define GLHELPER_H
struct GLHelper
	/*! GLHelper structure to encapsulate initialization stuff ...
	*/
{
	static bool init(GLint w, GLint h, std::string t);
	static void cleanup();

	// Callbacks
	static void error_cb(int error, char const* description);
	static void fbsize_cb(GLFWwindow* ptr_win, int width, int height);
	// I/O callbacks
	static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
	static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
	static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);
	static void print_specs();

	static void update_time(double fpsCalcInt = 1.0);

	static GLint width, height;
	static GLdouble fps;
	static GLdouble delta_time;
	static std::string title;
	static GLFWwindow* ptr_window;
	static GLboolean keystate_T;
	static GLboolean keystate_M;
	static GLboolean keystate_A;
};

#endif /* GLHELPER_H */
