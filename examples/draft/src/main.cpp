////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/application.h"

int main(int, char**)
{
    Application app;
    app.initialize();
    app.createWindow("Foo", 640, 640);
    //app.createWindow("Bar", 640, 640);
    app.createCommandQueue();
    app.run();

    return 0;
}
