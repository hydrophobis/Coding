use std::sync::{Arc, Mutex, atomic::{AtomicBool, AtomicUsize, Ordering}};
use std::thread;
use std::time::Instant;

fn generate_combinations(
    char_set: &str,
    max_length: usize,
    password: &str,
    current_string: String,
    guesses: Arc<AtomicUsize>,
    found: Arc<AtomicBool>,
) {
    if found.load(Ordering::Acquire) {
        return;
    }

    if current_string.len() == max_length {
        return;
    }

    for c in char_set.chars() {
        if found.load(Ordering::Acquire) {
            return;
        }

        let mut new_string = current_string.clone();
        new_string.push(c);
        guesses.fetch_add(1, Ordering::Relaxed);

        if new_string == password {
            if !found.load(Ordering::Acquire) {
                found.store(true, Ordering::Release);
                println!("Found {} with {} guesses", password, guesses.load(Ordering::Acquire));
            }
            return;
        }

        generate_combinations(
            char_set,
            max_length,
            password,
            new_string,
            guesses.clone(),
            found.clone(),
        );
    }
}

fn main() {
    let char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~";
    let mut password = String::new();
    let mut max_length = String::new();

    println!("Password: ");
    std::io::stdin().read_line(&mut password).unwrap();
    let password = password.trim();
    
    println!("Length: ");
    std::io::stdin().read_line(&mut max_length).unwrap();
    let max_length: usize = max_length.trim().parse().unwrap();

    let start = Instant::now();

    let num_threads = num_cpus::get();
    let mut threads = vec![];
    let guesses = Arc::new(AtomicUsize::new(0));
    let found = Arc::new(AtomicBool::new(false));

    for _ in 0..num_threads {
        let char_set = char_set.to_string();
        let password = password.to_string();
        let guesses = guesses.clone();
        let found = found.clone();

        threads.push(thread::spawn(move || {
            generate_combinations(
                &char_set,
                max_length,
                &password,
                String::new(),
                guesses,
                found,
            );
        }));
    }

    for t in threads {
        t.join().unwrap();
    }

    let elapsed = start.elapsed();
    println!("Time elapsed: {:.3} s", elapsed.as_secs_f64());
}
