#include "Viewer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<chrono>
// Internal global variables used for glfw event handling
static Viewer* __viewer;
static int highdpi = 1;
static double scroll_x = 0;
static double scroll_y = 0;


static void glfw_mouse_press(GLFWwindow* /*window*/, int button, int action, int modifier)
{
    Viewer::MouseButton mb;

    if (button == GLFW_MOUSE_BUTTON_1)
    {
        mb = Viewer::MouseButton::Left;
    }
    else if (button == GLFW_MOUSE_BUTTON_2)
    {
        mb = Viewer::MouseButton::Right;
    }
    else // if (button == GLFW_MOUSE_BUTTON_3)
    {
        mb = Viewer::MouseButton::Middle;
    }

    if (action == GLFW_PRESS)
    {
        __viewer->mouse_down(mb, modifier);
    }
    else
    {
        __viewer->mouse_up(mb, modifier);
    }
}

static void glfw_error_callback(int /*error*/, const char* description)
{
    fputs(description, stderr);
}

static void glfw_char_mods_callback(GLFWwindow* /*window*/, unsigned int codepoint, int modifier)
{
    __viewer->key_pressed(codepoint, modifier);
}

static void glfw_key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int modifier)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (action == GLFW_PRESS)
    {
        __viewer->key_down(key, modifier);
    }
    else if (action == GLFW_RELEASE)
    {
        __viewer->key_up(key, modifier);
    }
    else if (action == GLFW_REPEAT)
    {
        __viewer->key_repeat(key, modifier);
    }
}

static void glfw_window_size(GLFWwindow* window, int width, int height)
{
    int w = (int)(width * highdpi);
    int h = (int)(height * highdpi);

    __viewer->post_resize(w, h);
}

static void glfw_mouse_move(GLFWwindow* /*window*/, double x, double y)
{
    __viewer->mouse_move((int)x * highdpi, (int)y * highdpi);
}

static void glfw_mouse_scroll(GLFWwindow* /*window*/, double x, double y)
{
    using namespace std;
    scroll_x += x;
    scroll_y += y;

    __viewer->mouse_scroll((float)y);
}

static void glfw_drop_callback(GLFWwindow* /*window*/, int /*count*/, const char** /*filenames*/)
{

}



[[maybe_unused]] int Viewer::launch(bool resizable /*= true*/, bool fullscreen /*= false*/, bool maximize /*= false*/,
    const std::string& name, int windowWidth /*= 0*/, int windowHeight /*= 0*/)
{
    if (launch_init(resizable, fullscreen, maximize, name, windowWidth, windowHeight) == EXIT_FAILURE)
    {
        launch_shut();
        return EXIT_FAILURE;
    }
    launch_rendering(true);
    launch_shut();
    return EXIT_SUCCESS;
}

int Viewer::launch_init(bool resizable, bool fullscreen, bool maximize,
    const std::string& name, int windowWidth, int windowHeight)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Error: Could not initialize OpenGL context");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if (fullscreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(mode->width, mode->height, name.c_str(), monitor, nullptr);
        windowWidth = mode->width;
        windowHeight = mode->height;
    }
    else
    {
      
         if (windowWidth <= 0)
        {
            windowWidth = 1280;
        }
  
         if (windowHeight <= 0)
        {
            windowHeight = 800;
        }
        window = glfwCreateWindow(windowWidth, windowHeight, name.c_str(), nullptr, nullptr);
        if (maximize)
        {
            glfwMaximizeWindow(window);
        }
    }
    if (!window)
    {
        fprintf(stderr, "Error: Could not create GLFW window");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL and its extensions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load OpenGL and its extensions\n");
        return (-1);
    }
#if defined(DEBUG) || defined(_DEBUG)
    printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);
    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version received: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Initialize one and only reference
    __viewer = this;

    // Register callbacks
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetCursorPosCallback(window, glfw_mouse_move);
    glfwSetWindowSizeCallback(window, glfw_window_size);
    glfwSetMouseButtonCallback(window, glfw_mouse_press);
    glfwSetScrollCallback(window, glfw_mouse_scroll);
    glfwSetCharModsCallback(window, glfw_char_mods_callback);
    glfwSetDropCallback(window, glfw_drop_callback);

    // Handle retina displays (windows and mac)
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    int width_window, height_window;
    glfwGetWindowSize(window, &width_window, &height_window);
    highdpi = windowWidth / width_window;
    glfw_window_size(window, width_window, height_window);

    // Initialize viewer
    init();


    return EXIT_SUCCESS;
}

void Viewer::launch_rendering(bool loop)
{
    // glfwMakeContextCurrent(window);
    // Rendering loop
    bool first = true;
    const int num_extra_frames = 5;
    int frame_counter = 0;
    while (!glfwWindowShouldClose(window))
    {
        double tic = std::chrono::duration<double>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        draw(first);
        if (first)
        {
            first = false;
        }
        glfwSwapBuffers(window);
        if (is_animating || frame_counter++ < num_extra_frames)
        {
            glfwPollEvents();
            // In microseconds
            double toc = std::chrono::duration<double>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            double duration = 1000000. * (toc - tic);

        }
        else
        {
            glfwWaitEvents();
            frame_counter = 0;
        }
        if (!loop)
        {
            return;
        }

#ifdef __APPLE__
        static bool first_time_hack = true;
        if (first_time_hack) {
            glfwHideWindow(window);
            glfwShowWindow(window);
            first_time_hack = false;
        }
#endif
    }
}

void Viewer::launch_shut()
{

   // core().shut(); // Doesn't do anything
    shutdown_plugins();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Viewer::init()
{
   // core().init(); // Doesn't do anything

    if (callback_init)
    {
        if (callback_init(*this))
        {
            return;
        }
    }

    init_plugins();
}

void Viewer::init_plugins()
{
    // Init all plugins
    for (auto& plugin : plugins)
    {
        plugin->init(this);
    }
}

void Viewer::shutdown_plugins()
{
    for (auto& plugin : plugins)
    {
        plugin->shutdown();
    }
}

Viewer::Viewer()
{
    window = nullptr;


    // Temporary variables initialization
    down = false;
    hack_never_moved = true;
    scroll_position = 0.0f;

    // C-style callbacks
    callback_init = nullptr;
    callback_pre_draw = nullptr;
    callback_post_draw = nullptr;
    callback_mouse_down = nullptr;
    callback_mouse_up = nullptr;
    callback_mouse_move = nullptr;
    callback_mouse_scroll = nullptr;
    callback_key_pressed = nullptr;
    callback_key_down = nullptr;
    callback_key_up = nullptr;
    callback_key_repeat = nullptr;

    callback_init_data = nullptr;
    callback_pre_draw_data = nullptr;
    callback_post_draw_data = nullptr;
    callback_mouse_down_data = nullptr;
    callback_mouse_up_data = nullptr;
    callback_mouse_move_data = nullptr;
    callback_mouse_scroll_data = nullptr;
    callback_key_pressed_data = nullptr;
    callback_key_down_data = nullptr;
    callback_key_up_data = nullptr;
    callback_key_repeat_data = nullptr;
}




Viewer::~Viewer() = default;



////////////////////////////////////////////////////////////////////////////////
// Callbacks
////////////////////////////////////////////////////////////////////////////////

bool Viewer::key_pressed(unsigned int key, int modifiers)
{
    for (auto& plugin : plugins)
    {
        if (plugin->key_pressed(key, modifiers))
        {
            return true;
        }
    }

    if (callback_key_pressed)
    {
        if (callback_key_pressed(*this, key, modifiers))
        {
            return true;
        }
    }

    return false;
}

bool Viewer::key_down(int key, int modifiers)
{
    for (auto& plugin : plugins)
    {
        if (plugin->key_down(key, modifiers))
        {
            return true;
        }
    }

    if (callback_key_down)
    {
        if (callback_key_down(*this, key, modifiers))
        {
            return true;
        }
    }

    return false;
}

bool Viewer::key_up(int key, int modifiers)
{
    for (auto& plugin : plugins)
    {
        if (plugin->key_up(key, modifiers))
        {
            return true;
        }
    }

    if (callback_key_up)
    {
        if (callback_key_up(*this, key, modifiers))
        {
            return true;
        }
    }

    return false;
}

bool Viewer::key_repeat(int key, int modifiers)
{
    for (auto& plugin : plugins)
    {
        if (plugin->key_repeat(key, modifiers))
        {
            return true;
        }
    }

    if (callback_key_repeat)
    {
        if (callback_key_repeat(*this, key, modifiers))
        {
            return true;
        }
    }

    return false;
}



bool Viewer::mouse_down(MouseButton button, int modifier)
{
    // Remember mouse location at down even if used by callback/plugin
    down_mouse_x = current_mouse_x;
    down_mouse_y = current_mouse_y;

    for (auto& plugin : plugins)
    {
        if (plugin->mouse_down(static_cast<int>(button), modifier))
        {
            return true;
        }
    }

    if (callback_mouse_down)
    {
        if (callback_mouse_down(*this, static_cast<int>(button), modifier))
        {
            return true;
        }
    }

    down = true;


   

    // Initialization code for the trackball
    Eigen::RowVector3d center;
  
    mouse_mode = MouseMode::Rotation;

    switch (button)
    {
    case MouseButton::Left:
        break;

    case MouseButton::Right:
        break;

    default:
        break;
    }

    return true;
}

bool Viewer::mouse_up(MouseButton button, int modifier)
{
    down = false;

    for (auto& plugin : plugins)
    {
        if (plugin->mouse_up(static_cast<int>(button), modifier))
        {
            return true;
        }
    }

    if (callback_mouse_up)
    {
        if (callback_mouse_up(*this, static_cast<int>(button), modifier))
        {
            return true;
        }
    }

    mouse_mode = MouseMode::None;

    return true;
}

bool Viewer::mouse_move(int mouse_x, int mouse_y)
{
  
    for (auto& plugin : plugins)
    {
        if (plugin->mouse_move(mouse_x, mouse_y))
        {
            return true;
        }
    }

    if (callback_mouse_move)
    {
        if (callback_mouse_move(*this, mouse_x, mouse_y))
        {
            return true;
        }
    }

    if (down)
    {
        // We need the window height to transform the mouse click coordinates
        // into viewport-mouse-click coordinates for trackball and
        // two_axis_valuator_fixed_up
        int width_window, height_window;
        glfwGetFramebufferSize(window, &width_window, &height_window);
        switch (mouse_mode)
        {
        case MouseMode::Rotation:
        {
  
            break;
        }

        case MouseMode::Translation:
        {
          
            break;
        }
        case MouseMode::Zoom:
        {
            break;
        }

        default:
            break;
        }
    }
    return true;
}

bool Viewer::mouse_scroll(float delta_y)
{
    // Direct the scrolling operation to the appropriate viewport
    // (unless the core selection is locked by an ongoing mouse interaction).
   
    scroll_position += delta_y;

    for (auto& plugin : plugins)
    {
        if (plugin->mouse_scroll(delta_y))
        {
            return true;
        }
    }

    if (callback_mouse_scroll)
    {
        if (callback_mouse_scroll(*this, delta_y))
        {
            return true;
        }
    }


    return true;
}

void Viewer::draw(bool first)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    int width_window, height_window;
    glfwGetWindowSize(window, &width_window, &height_window);

    auto highdpi_tmp = (width_window == 0 || width == 0) ? highdpi : (width / width_window);

    if (fabs(highdpi_tmp - highdpi) > 1e-8)
    {
        post_resize(width, height);
        highdpi = highdpi_tmp;
    }


    for (auto& plugin : plugins)
    {
        if (plugin->pre_draw(first))
        {
            break;
        }
    }

    if (callback_pre_draw)
    {
        if (callback_pre_draw(*this))
        {

        }
    }



    //pre-draw finish
    DrawAction();
    //post-draw action



    for (auto& plugin : plugins)
    {
        if (plugin->post_draw(first))
        {
            break;
        }
    }

    if (callback_post_draw)
    {
        if (callback_post_draw(*this))
        {

        }
    }
}



void Viewer::post_resize(int w, int h){

 
    for (auto& plugin : plugins)
    {
        plugin->post_resize(w, h);
    }
    if (callback_post_resize)
    {
        callback_post_resize(*this, w, h);
    }
}




