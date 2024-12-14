// Config file to declare misc values,
// this probably isnt great practice since one change takes forever to re-compile but whatever
#pragma once
#include <GLFW/glfw3.h>
#include "build.h"

#define SHOW_DEARIMGUI_DEMO 0

// OpenGL Definitions
#define CONF_GL_VERSION_MAJOR 3
#define CONF_GL_VERSION_MINOR 3
#define CONF_GL_PROFILE GLFW_OPENGL_CORE_PROFILE

// Window Definitions
#define CONF_WINDOW_WIDTH 800
#define CONF_WINDOW_HEIGHT 600
#define CONF_WINDOW_TITLE "Verlet Integration"

// Application Definitions
#define CONF_RESOURCE_LOCATION RES_DIR
#define CONF_MAX_CHAR_SIZE 512
#if VT_DEBUG
	#define CONF_BUILD_TYPE "Debug"
#elif VT_RELEASE
	#define CONF_BUILD_TYPE "Release"
#elif VT_DIST
	#define CONF_BUILD_TYPE "Distribution"
#endif // VT_DEBUG

// Camera Definitions
#define CONF_CAMERA_SPEED 2.5f
#define CONF_CAMERA_SENS 0.1f
#define CONF_CAMERA_FIELD_OF_VIEW 75.0f
#define CONF_CAMERA_NEAR_PLANE 0.1f
#define CONF_CAMERA_FAR_PLANE 1000.0f

// Debug Definitions
#define CONF_DEBUG_VERT_SHADER "res:/shaders/debug/debug.vert"
#define CONF_DEBUG_FRAG_SHADER "res:/shaders/debug/debug.frag"
#define DEBUG_FEATURES VT_DEBUG || VT_RELEASE