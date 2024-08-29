use rayon::prelude::*;
use std::sync::{Arc, Mutex};
use std::time::Instant;

fn generate_combinations(
    charset: &[char],
    max_len: usize,
    target_password: &str,
) -> Option<(String, u64)> {
    let start = Instant::now();
    let guesses = Arc::new(Mutex::new(0));

    // Function to generate combinations
    fn recurse(
        charset: &[char],
        max_len: usize,
        prefix: String,
        target_password: &str,
        guesses: Arc<Mutex<u64>>,
        start: Instant,
    ) -> Option<String> {
        if prefix.len() > max_len {
            return None;
        }

        if prefix == target_password {
            return Some(prefix);
        }

        for &c in charset {
            let new_prefix = format!("{}{}", prefix, c);
            {
                let mut count = guesses.lock().unwrap();
                *count += 1;
            }
            if let Some(result) = recurse(charset, max_len, new_prefix, target_password, guesses.clone(), start) {
                return Some(result);
            }
        }
        None
    }

    let result = (1..=max_len)
        .into_par_iter()
        .find_map_any(|len| {
            let guesses_inner = Arc::new(Mutex::new(0));
            let result = recurse(
                charset,
                len,
                String::new(),
                target_password,
                guesses_inner.clone(),
                start,
            );
            let count = *guesses_inner.lock().unwrap();
            if result.is_some() {
                Some((result.unwrap(), count))
            } else {
                None
            }
        });

    let elapsed = start.elapsed();
    result.map(|(found, count)| (found, count))
}

fn main() {
    let charset: Vec<char> = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~".chars().collect();
    
    let mut password = String::new();
    let mut max_len = String::new();

    println!("Password: ");
    std::io::stdin().read_line(&mut password).expect("Failed to read input");
    let password = password.trim();

    println!("Length: ");
    std::io::stdin().read_line(&mut max_len).expect("Failed to read input");
    let max_len: usize = max_len.trim().parse().expect("Invalid length");

    let start = Instant::now();
    match generate_combinations(&charset, max_len, password) {
        Some((found, count)) => {
            let elapsed = start.elapsed();
            println!("Found {} with {} guesses in {:.2?} seconds", found, count, elapsed);
        }
        None => println!("Failed to find the password."),
    }
}
