#ifndef APP_H
#define APP_H

#pragma once

#include "iamgui/imgui.h"
#include "iamgui/imgui_impl_glfw.h"
#include "iamgui/imgui_impl_opengl3.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constants.h"

#include "Camera.h"


class App
{
public:
	GLFWwindow* window = NULL;


public:
	void init();
};

#endif