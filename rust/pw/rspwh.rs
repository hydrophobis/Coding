use sha2::{Sha256, Digest};
use std::iter::repeat;
use std::time::Instant;

fn brute_force(target_hash: &str, charset: &[char], max_len: usize) -> Option<(String, i32)> {
    let mut guess = String::new();
    let mut guesses = 0;
    for len in 1..=max_len {
        guess.clear();
        let mut indices = vec![0; len];
        loop {
            for &i in indices.iter() {
                guess.push(charset[i]);
            }

            let mut hasher = Sha256::new();
            hasher.update(&guess);
            let hash_result = hasher.finalize();
            let hash_str = format!("{:x}", hash_result);

            guesses += 1;
            if hash_str == target_hash {
                return Some((guess, guesses));
            }

            if !increment_indices(&mut indices, charset.len()) {
                break;
            }
            guess.clear();
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
    let charset: Vec<char> = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789".chars().collect();
    let target_password = "for";
    let max_len = 3;

    // Compute the target hash
    let mut hasher = Sha256::new();
    hasher.update(target_password);
    let target_hash = format!("{:x}", hasher.finalize());

    let start_time = Instant::now();
    match brute_force(&target_hash, &charset, max_len) {
        Some((found, count)) => println!("Password guessed: {} in {} guesses and {:.2?}", found, count, start_time.elapsed()),
        None => println!("Failed to guess the password."),
    }
}
