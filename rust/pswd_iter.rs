use std::iter::repeat;
use std::time::Instant;

fn brute_force(target: &str, charset: &[char], max_len: usize) -> Option<String> {
    let mut guess = String::new();
    for len in 1..=max_len {
        guess.clear();
        let mut indices = vec![0; len];
        loop {
            for &i in indices.iter() {
                guess.push(charset[i]);
            }
            if guess == target {
                return Some(guess);
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
    let charset: Vec<char> = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789".chars().collect();
    let target_password = "apple";
    let max_len = 5;

    let start_time = Instant::now();
    match brute_force(target_password, &charset, max_len) {
        Some(found) => println!("Password guessed: {} in {:.2?}", found, start_time.elapsed()),
        None => println!("Failed to guess the password."),
    }
}
