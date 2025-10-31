use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    resource: Resource,
}

#[derive(Clone, Subcommand)]
enum Resource {
    Memory { arg: String },
    Thread { num: u32 },
}

#[derive(Clone)]
struct Memory {
    size: u32,
    multiplier: u32,
}

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

        let size = size_str.strip_suffix(suffix).unwrap().parse::<u32>()?;

        // drop(res);

        Ok(Memory { size, multiplier })
    }

    fn execute(self) {
        let total_size = self.size * self.multiplier;
        println!("Allocating {} bytes of memory.", total_size);

        unsafe {
            let layout = std::alloc::Layout::from_size_align(total_size as usize, 8).unwrap();
            let ptr = std::alloc::alloc(layout);
            if ptr.is_null() {
                panic!("Memory allocation failed");
            }

            // dummy usage of allocated memory
            for i in 0..total_size as usize {
                *ptr.add(i) = 0;
            }

            std::alloc::dealloc(ptr, layout);
        }
    }
}

fn main() {
    let cli = Cli::parse();
    println!("Hello, world!");

    match cli.resource {
        Resource::Memory { .. } => {
            Memory::from_resource(cli.resource)
                .unwrap_or_else(|e| {
                    eprintln!("Error: {e}");
                    std::process::exit(1);
                })
                .execute();

            println!("Done!");
        }

        Resource::Thread { .. } => {
            todo!("Implement thread resource allocation");
        }
    }
}
