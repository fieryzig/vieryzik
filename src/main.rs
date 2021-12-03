use std::sync::Arc;
use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::{Window,WindowBuilder},
    platform::run_return::EventLoopExtRunReturn,
};
use vulkano::instance::{
    Instance,
    InstanceExtensions,
    ApplicationInfo,
    Version,
};

struct App {
    instance: Arc<Instance>,
    event_loop: EventLoop<()>,
    window: Window,
}

impl App {
    pub fn init() -> Self {
        let instance = Self::create_instance();
        let (event_loop, window) = Self::init_window();

        Self {
            instance,
            event_loop,
            window,
        }
    }

    fn init_window() -> (EventLoop<()>, Window) {
        let event_loop = EventLoop::new();
        let window = WindowBuilder::new().build(&event_loop).unwrap();
        (event_loop, window)
    }

    fn create_instance() -> Arc<Instance> {
        let supported_extensions = InstanceExtensions::supported_by_core()
            .expect("failed to retrieve supported extensions");
        println!("Supported extensions: {:?}", supported_extensions);

        let app_info = ApplicationInfo {
            application_name: Some("Hello Triangle".into()),
            application_version: Some(Version { major: 1, minor: 1, patch: 0 }),
            engine_name: Some("No Engine".into()),
            engine_version: Some(Version { major: 1, minor: 1, patch: 0 }),
        };

        let required_extensions = vulkano_win::required_extensions();
        Instance::new(Some(&app_info), Version::V1_1, &required_extensions, None)
            .expect("failed to create Vulkan instance")
    }

    pub fn main_loop(&mut self) {
        self.event_loop.run_return(|event, _, control_flow| {
            // ControlFlow::Poll continuously runs the event loop, even if the OS hasn't
            // dispatched any events. This is ideal for games and similar applications.
            *control_flow = ControlFlow::Poll;

            // ControlFlow::Wait pauses the event loop if no events are available to process.
            // This is ideal for non-game applications that only update in response to user
            // input, and uses significantly less power/CPU time than ControlFlow::Poll.
            *control_flow = ControlFlow::Wait;

            match event {
                Event::WindowEvent {
                    event: WindowEvent::CloseRequested,
                    ..
                } => {
                    println!("The close button was pressed; stopping");
                    *control_flow = ControlFlow::Exit
                },
                Event::MainEventsCleared => {
                    // Application update code.

                    // Queue a RedrawRequested event.
                    //
                    // You only need to call this if you've determined that you need to redraw, in
                    // applications which do not always need to. Applications that redraw continuously
                    // can just render here instead.
                    self.window.request_redraw();
                },
                Event::RedrawRequested(_) => {
                    // Redraw the application.
                    //
                    // It's preferable for applications that do not render continuously to render in
                    // this event rather than in MainEventsCleared, since rendering in here allows
                    // the program to gracefully handle redraws requested by the OS.
                },
                _ => ()
            }
        });
    }
}

fn main() {
    let mut app = App::init();
    app.main_loop();
}
