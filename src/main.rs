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
    layers_list,
};
use vulkano::instance::debug::{
    DebugCallback,
    MessageType,
    MessageSeverity,
};

const VALIDATION_LAYERS: &[&str] =  &[
    "VK_LAYER_LUNARG_standard_validation"
];

#[cfg(all(debug_assertions))]
const ENABLE_VALIDATION_LAYERS: bool = true;
#[cfg(not(debug_assertions))]
const ENABLE_VALIDATION_LAYERS: bool = false;

struct App {
    instance: Arc<Instance>,
    event_loop: EventLoop<()>,
    window: Window,
    debug_callback: Option<DebugCallback>,
}

impl App {
    pub fn init() -> Self {
        let instance = Self::create_instance();
        let (event_loop, window) = Self::init_window();
        let debug_callback = Self::setup_debug_callback(&instance);

        Self {
            instance,
            event_loop,
            window,
            debug_callback,
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

        let mut required_extensions = vulkano_win::required_extensions();
        if ENABLE_VALIDATION_LAYERS {
            required_extensions.ext_debug_utils = true;
        }
        if ENABLE_VALIDATION_LAYERS && Self::check_validation_layer_support() {
            Instance::new(Some(&app_info), Version::V1_1, &required_extensions, VALIDATION_LAYERS.iter().cloned())
                .expect("failed to create Vulkan instance")
        } else {
            Instance::new(Some(&app_info), Version::V1_1, &required_extensions, None)
                .expect("failed to create Vulkan instance")
        }
    }

    fn check_validation_layer_support() -> bool {
        let layers: Vec<_> = layers_list().unwrap().map(|l| l.name().to_owned()).collect();
        println!("layers: {:?}", layers);
        VALIDATION_LAYERS.iter()
            .all(|layer_name| layers.contains(&layer_name.to_string()))
    }

    fn setup_debug_callback(instance: &Arc<Instance>) -> Option<DebugCallback> {
        if !ENABLE_VALIDATION_LAYERS  {
            return None;
        }
        let severity = MessageSeverity {
            error: true,
            warning: true,
            information: true,
            verbose: true,
        };
        let ty = MessageType::all();
        DebugCallback::new(&instance, severity, ty, |msg| {
            let severity = if msg.severity.error {
                "error"
            } else if msg.severity.warning {
                "warning"
            } else if msg.severity.information {
                "information"
            } else if msg.severity.verbose {
                "verbose"
            } else {
                panic!("no-impl");
            };

            let ty = if msg.ty.general {
                "general"
            } else if msg.ty.validation {
                "validation"
            } else if msg.ty.performance {
                "performance"
            } else {
                panic!("no-impl");
            };

            println!(
                "{:#?} {} {}: {}",
                msg.layer_prefix, ty, severity, msg.description
            );
        }).ok()
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
