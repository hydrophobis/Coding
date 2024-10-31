use sha2::{Sha256, Digest};
use std::env::args;
use std::iter::repeat;
use std::time::Instant;

fn brute_force(target_hash: &str, charset: &[char], max_len: usize) -> Option<(String, u64)> {
    let mut guess = String::new();
    let mut guesses = 0u64;
    for len in 1..=max_len {
        guess.clear();
        let mut indices = vec![0; len];
        loop {
            // Generate the guess from indices
            guess = indices.iter().map(|&i| charset[i]).collect::<String>();

            // Compute the hash of the guess
            let mut hasher = Sha256::new();
            hasher.update(&guess);
            let hash_result = hasher.finalize();
            let hash_str = format!("{:x}", hash_result);

            guesses += 1;
            if hash_str == target_hash.to_lowercase() {
                return Some((guess, guesses));
            }

            // Increment the indices for the next combination
            if !increment_indices(&mut indices, charset.len()) {
                break;
            }
        }
    }
    None
}

fn increment_indices(indices: &mut [usize], base: usize) -> bool {
    for i in (0..indices.len()).rev() {
        if indices[i] + 1 < base {
            indices[i] += 1;
            return true;
        } else {
            indices[i] = 0;
        }
    }
    false
}

fn main() {
    let args: Vec<String> = args().collect();
    let charset: Vec<char> = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789!@#$%^&*()_-".chars().collect();
    let target_hash = &args[1];

    let max_len = 5; // Increase this for more extensive searches

    let start_time = Instant::now();
    match brute_force(&target_hash, &charset, max_len) {
        Some((found, count)) => println!("Password guessed: {}({}) in {} guesses and {:.2?}", target_hash, found, count, start_time.elapsed()),
        None => println!("Failed to guess the password."),
    }
}
