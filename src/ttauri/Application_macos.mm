

#include "Application_macos.hpp"
#import <Cocoa/Cocoa.h>

namespace tt {

[[nodiscard]] static std::vector<std::string> passArguments(int argc, char const * const *argv) noexcept
{
    std::vector<std::string> arguments;

    for (int i = 0; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }

    return arguments;
}

Application_macos::Application_macos(std::shared_ptr<ApplicationDelegate> delegate, int argc, char const * const *argv) :
    Application_base(std::move(delegate), passArguments(argc, argv), nullptr, 0),
    argc(argc), argv(argv)
{
#if defined(TTAURI_GUI_ENABLED)
    @autoreleasepool {
        [NSApplication sharedApplication];

        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        // Get the main path the Resources folder of the bundle by requesting a known existing file.
        //auto _resourceFile = [[NSBundle mainBundle] pathForResource:@"BackingPipeline.frag" ofType:@"spv"];
        resourceDir = resourceFile.parent_path();

        id menubar = [NSMenu new];
        id appMenuItem = [NSMenuItem new];
        [menubar addItem:appMenuItem];
        [NSApp setMainMenu:menubar];
        id appMenu = [NSMenu new];
        id appName = [[NSProcessInfo processInfo] processName];
        id quitTitle = [@"Quit " stringByAppendingString:appName];
        id quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
            action:@selector(terminate:) keyEquivalent:@"q"];
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];

        id window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 200, 200)
            styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO];
                
        [window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
        [window setTitle:appName];
        [window makeKeyAndOrderFront:nil];
        
        [NSApp activateIgnoringOtherApps:YES];
    }
#endif
}

Application_macos::~Application_macos()
{
}

void Application_macos::runFromMainLoop(std::function<void()> function)
{

    ttlet functionP = new std::function<void()>(std::move(function));
    tt_assert(functionP);

    // XXX post a message to the main thread loop.
    //auto r = PostThreadMessageW(mainThreadID, WM_APP_CALL_FUNCTION, 0, reinterpret_cast<LPARAM>(functionP));
    //tt_assert(r != 0);
}

int Application_macos::loop()
{
    startingLoop();

    [NSApp run];

    return 0;
}

}

