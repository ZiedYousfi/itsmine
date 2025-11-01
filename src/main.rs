use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    resource: Resource,
    #[arg(short, long, default_value_t = false)]
    verbose: bool,
}

#[derive(Clone, Subcommand)]
enum Resource {
    Memory { arg: String },
    Thread { num: u32 },
}

#[derive(Clone)]
struct Memory {
    size: u64,
    multiplier: u64,
}

struct Thread(u32);

impl Memory {
    fn from_resource(res: Resource) -> Result<Self, anyhow::Error> {
        match res {
            Resource::Memory { .. } => {}
            Resource::Thread { .. } => {
                return Err(anyhow::anyhow!(
                    "Expected Memory resource, got Thread resource"
                ));
            }
        }

        let size_str = match res {
            Resource::Memory { arg } => arg,
            _ => unreachable!(),
        };

        let (multiplier, suffix) = if size_str.ends_with('B') {
            (1, 'B')
        } else if size_str.ends_with('K') {
            (1024, 'K')
        } else if size_str.ends_with('M') {
            (1024 * 1024, 'M')
        } else if size_str.ends_with('G') {
            (1024 * 1024 * 1024, 'G')
        } else {
            return Err(anyhow::anyhow!(
                "Invalid memory size suffix. Use B, K, M, or G."
            ));
        };

        let size = size_str
            .strip_suffix(suffix)
            .ok_or_else(|| anyhow::anyhow!("Invalid memory size {size_str}"))
            .and_then(|s| {
                s.parse::<u64>()
                    .map_err(|e| anyhow::anyhow!("Failed to parse memory size '{s}': {e}"))
            })
            .expect("Failed to parse memory size");

        // drop(res);

        Ok(Memory { size, multiplier })
    }

    fn execute(self) {
        let total_size = self.size * self.multiplier;
        assert!(total_size > 0, "Memory size must be greater than 0");
        log::info!("Allocating {} bytes of memory.", total_size);

        unsafe {
            let layout = std::alloc::Layout::from_size_align(total_size as usize, 8).unwrap();
            let ptr = std::alloc::alloc(layout);
            if ptr.is_null() {
                panic!("Memory allocation failed");
            }

            // dummy usage of allocated memory
            log::info!("Using allocated memory...");
            for i in 0..total_size as usize {
                *ptr.add(i) = 0;
                log::info!("used byte {}", i);
            }
            log::info!("Memory allocation and usage complete.");

            std::alloc::dealloc(ptr, layout);
        }
    }
}

impl Thread {
    fn new(num: u32) -> Self {
        Thread(num)
    }

    fn from_resource(res: Resource) -> Result<Self, anyhow::Error> {
        match res {
            Resource::Thread { num } => Ok(Thread::new(num)),
            Resource::Memory { .. } => Err(anyhow::anyhow!(
                "Expected Thread resource, got Memory resource"
            )),
        }
    }

    fn execute(self) {
        log::info!("Spawning {} threads.", self.0);
        let mut handles = vec![];

        let (tx, rx) = std::sync::mpsc::channel::<u32>();

        for i in 0..self.0 {
            let tx = tx.clone();
            let handle = std::thread::spawn(move || {
                log::info!("Thread {} started.", i);
                let fib = fibonacci(30); // Example workload
                tx.send(fib).unwrap();
                log::info!("Thread {} finished. Fibonacci(30) = {}", i, fib);
            });
            handles.push(handle);
        }

        let mut results: Vec<u32> = vec![];

        for handle in handles {
            handle.join().expect("Thread panicked");
            let result = rx.recv().unwrap();
            results.push(result);
            log::info!("Received from thread: {}", result);
        }

        let first = results[0];

        for &x in results.iter() {
            if x != first {
                panic!("Inconsistent results from threads");
            }
        }
        log::info!("All threads completed.");
    }
}

fn fibonacci(n: u32) -> u32 {
    if n <= 1 {
        return n;
    }
    fibonacci(n - 1) + fibonacci(n - 2)
}

fn main() {
    let cli = Cli::parse();
    match cli.verbose {
        true => simple_logger::init_with_level(log::Level::Debug).unwrap(),
        false => simple_logger::init_with_level(log::Level::Error).unwrap(),
    }
    log::info!("Hello, world!");

    match cli.resource {
        Resource::Memory { .. } => {
            Memory::from_resource(cli.resource)
                .unwrap_or_else(|e| {
                    log::error!("Error: {e}");
                    std::process::exit(1);
                })
                .execute();

            log::info!("Done!");
        }

        Resource::Thread { .. } => {
            Thread::from_resource(cli.resource)
                .unwrap_or_else(|e| {
                    log::error!("Error: {e}");
                    std::process::exit(1);
                })
                .execute();
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    // Memory tests
    #[test]
    fn test_memory_allocation() {
        let memory = Memory {
            size: 1,
            multiplier: 1024,
        };
        memory.execute();
    }

    #[test]
    fn memory_from_resource_valid_b() {
        let res = Resource::Memory {
            arg: "100B".to_string(),
        };
        let memory = Memory::from_resource(res).unwrap();
        assert_eq!(memory.size, 100);
        assert_eq!(memory.multiplier, 1);
    }

    #[test]
    fn memory_from_resource_valid_g() {
        let res = Resource::Memory {
            arg: "2G".to_string(),
        };
        let memory = Memory::from_resource(res).unwrap();
        assert_eq!(memory.size, 2);
        assert_eq!(memory.multiplier, 1024 * 1024 * 1024);
    }

    #[test]
    fn memory_from_resource_invalid_no_suffix() {
        let res = Resource::Memory {
            arg: "10".to_string(),
        };
        let result = Memory::from_resource(res);
        assert!(result.is_err());
    }

    #[test]
    fn memory_from_resource_invalid_wrong_suffix() {
        let res = Resource::Memory {
            arg: "10X".to_string(),
        };
        let result = Memory::from_resource(res);
        assert!(result.is_err());
    }

    #[test]
    #[should_panic(
        expected = "Failed to parse memory size: Failed to parse memory size 'abc': invalid digit found in string"
    )]
    fn memory_from_resource_invalid_non_numeric() {
        let res = Resource::Memory {
            arg: "abcK".to_string(),
        };
        let result = Memory::from_resource(res);
        assert!(result.is_err());
    }

    #[test]
    fn memory_from_resource_zero_size() {
        let res = Resource::Memory {
            arg: "0K".to_string(),
        };
        let memory = Memory::from_resource(res).unwrap();
        assert_eq!(memory.size, 0);
        assert_eq!(memory.multiplier, 1024);
    }

    #[test]
    #[should_panic(expected = "Memory size must be greater than 0")]
    fn test_memory_execute_zero_size() {
        let memory = Memory {
            size: 0,
            multiplier: 1,
        };
        memory.execute();
    }

    #[test]
    fn test_memory_execute_large() {
        let memory = Memory {
            size: 1,
            multiplier: 1024 * 1024, // 1M
        };
        memory.execute();
    }

    #[test]
    fn memory_from_resource_invalid() {
        let res = Resource::Thread { num: 4 };
        let result = Memory::from_resource(res);
        assert!(result.is_err());
    }

    // Thread tests
    #[test]
    fn thread_from_resource_valid() {
        let res = Resource::Thread { num: 4 };
        let thread = Thread::from_resource(res).unwrap();
        assert_eq!(thread.0, 4);
    }

    #[test]
    fn thread_from_resource_invalid() {
        let res = Resource::Memory {
            arg: "100K".to_string(),
        };
        let result = Thread::from_resource(res);
        assert!(result.is_err());
    }
}
