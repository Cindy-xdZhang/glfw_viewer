#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry> 
#include <vector>
#include <string>
#include <iostream>
#include <functional>



struct GLFWwindow;
struct GLFWmonitor;


class ViewerPlugin;
class Viewer
{

public:
    // UI Enumerations
    enum class MouseButton
    {
        Left, Middle, Right
    };
    enum class MouseMode
    {
        None, Rotation, Zoom, Pan, Translation
    };

    [[maybe_unused]] int launch(
        bool resizable = true, bool fullscreen = false, bool maximize = false,
        const std::string& name = "viewer", int width = 0, int height = 0);

    [[maybe_unused]] int launch_init(
        bool resizable = true, bool fullscreen = false, bool maximize = false,
        const std::string& name = "viewer", int width = 0, int height = 0);

    void launch_rendering(bool loop = true);
    void launch_shut();

    void init();

    void init_plugins();
    void shutdown_plugins();

    Viewer();
    ~Viewer();

    // Callbacks
    bool key_pressed(unsigned int key, int modifier);
    bool key_down(int key, int modifier);
    bool key_up(int key, int modifier);
    bool key_repeat(int key, int modifier);
    bool mouse_down(MouseButton button, int modifier);
    bool mouse_up(MouseButton button, int modifier);
    bool mouse_move(int mouse_x, int mouse_y);
    bool mouse_scroll(float delta_y);

    // Draw everything
    void draw(bool first);


    // OpenGL context resize
    void resize(int w, int h); // explicitly set window size
    void post_resize(int w, int h); // external resize due to user interaction


    void setDrawCall(std::function<void(void)> drawcall) {
        DrawAction = drawcall;
    };



   

public:

    ////////////////////////////////////////////////////////////////////////////
    /// Member variables
    ////////////////////////////////////////////////////////////////////////////

    

    GLFWwindow* window;
    bool is_animating;
  
    // List of registered plugins
    std::vector<ViewerPlugin*> plugins;

    // Temporary data stored when the mouse button is pressed
    MouseMode mouse_mode = Viewer::MouseMode::None;
    Eigen::Quaternionf down_rotation;
    int current_mouse_x = 0;
    int current_mouse_y = 0;
    int down_mouse_x = 0;
    int down_mouse_y = 0;
    float down_mouse_z = 0;
    Eigen::Vector3f down_translation;
    bool down;
    bool hack_never_moved;

    // Keep track of the global position of the scroll wheel
    float scroll_position;

    // C++-style functions
    //
    // Returns **true** if action should be cancelled.
    std::function<bool(Viewer& viewer)> callback_init;
    std::function<bool(Viewer& viewer)> callback_pre_draw;
    std::function<bool(Viewer& viewer)> callback_post_draw;
    std::function<bool(Viewer& viewer, int w, int h)> callback_post_resize;
    std::function<bool(Viewer& viewer, int button, int modifier)> callback_mouse_down;
    std::function<bool(Viewer& viewer, int button, int modifier)> callback_mouse_up;
    std::function<bool(Viewer& viewer, int mouse_x, int mouse_y)> callback_mouse_move;
    std::function<bool(Viewer& viewer, float delta_y)> callback_mouse_scroll;
    std::function<bool(Viewer& viewer, unsigned int key, int modifiers)> callback_key_pressed;
    std::function<bool(Viewer& viewer, unsigned int key, int modifiers)> callback_key_down;
    std::function<bool(Viewer& viewer, unsigned int key, int modifiers)> callback_key_up;
    std::function<bool(Viewer& viewer, unsigned int key, int modifiers)> callback_key_repeat;
    std::function<void(void)>DrawAction;   //RenderAframe


    // Pointers to per-callback data
    void* callback_init_data;
    void* callback_pre_draw_data;
    void* callback_post_draw_data;
    void* callback_mouse_down_data;
    void* callback_mouse_up_data;
    void* callback_mouse_move_data;
    void* callback_mouse_scroll_data;
    void* callback_key_pressed_data;
    void* callback_key_down_data;
    void* callback_key_up_data;
    void* callback_key_repeat_data;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class ViewerPlugin
{

public:

    ViewerPlugin();

    virtual ~ViewerPlugin();
    virtual void init(Viewer* _viewer);

    // This function is called before shutdown
    virtual void shutdown();

    // This function is called before a mesh is loaded
    virtual bool load(const std::string& filename, bool only_vertices);

    // This function is called before a mesh is unloaded
    virtual bool unload();

    // This function is called before a mesh is saved
    virtual bool save(const std::string& filename, bool only_vertices);

    // This function is called when the scene is serialized
    virtual bool serialize(std::vector<char>& buffer) const;

    // This function is called when the scene is deserialized
    virtual bool deserialize(const std::vector<char>& buffer);

    // Runs immediately after a new mesh has been loaded.
    virtual bool post_load();

    // This function is called before the draw procedure of Preview3D
    virtual bool pre_draw(bool first);

    // This function is called after the draw procedure of Preview3D
    virtual bool post_draw(bool first);

    // This function is called after the window has been resized
    virtual bool post_resize(int w, int h);

    // This function is called when the mouse button is pressed
    // - button can be GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON or GLUT_RIGHT_BUTTON
    // - modifiers is a bitfield that might one or more of the following bits Preview3D::NO_KEY, Preview3D::SHIFT, Preview3D::CTRL, Preview3D::ALT;
    virtual bool mouse_down(int button, int modifier);

    // This function is called when the mouse button is released
    // - button can be GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON or GLUT_RIGHT_BUTTON
    // - modifiers is a bitfield that might one or more of the following bits Preview3D::NO_KEY, Preview3D::SHIFT, Preview3D::CTRL, Preview3D::ALT;
    virtual bool mouse_up(int button, int modifier);

    // This function is called every time the mouse is moved
    // - mouse_x and mouse_y are the new coordinates of the mouse pointer in screen coordinates
    virtual bool mouse_move(int mouse_x, int mouse_y);

    // This function is called every time the scroll wheel is moved
    // Note: this callback is not working with every glut implementation
    virtual bool mouse_scroll(float delta_y);

    // This function is called when a keyboard key is pressed. Unlike key_down
    // this will reveal the actual character being sent (not just the physical
    // key)
    // - modifiers is a bitfield that might one or more of the following bits Preview3D::NO_KEY, Preview3D::SHIFT, Preview3D::CTRL, Preview3D::ALT;
    virtual bool key_pressed(unsigned int key, int modifiers);

    // This function is called when a keyboard key is down
    // - modifiers is a bitfield that might one or more of the following bits Preview3D::NO_KEY, Preview3D::SHIFT, Preview3D::CTRL, Preview3D::ALT;
    virtual bool key_down(int key, int modifiers);

    // This function is called when a keyboard key is release
    // - modifiers is a bitfield that might one or more of the following bits Preview3D::NO_KEY, Preview3D::SHIFT, Preview3D::CTRL, Preview3D::ALT;
    virtual bool key_up(int key, int modifiers);

    // This function is called when a keyboard key is held down until it repeats
    // - modifiers is a bitfield that might one or more of the following bits Preview3D::NO_KEY, Preview3D::SHIFT, Preview3D::CTRL, Preview3D::ALT;
    virtual bool key_repeat(int key, int modifiers);

protected:

    // Pointer to the main Viewer class
    Viewer* mViewer;

    // Plugin name
    std::string mName;
};
