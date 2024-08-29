use std::time::Instant;

fn generate_combinations(
    char_set: &str,
    max_length: usize,
    password: &str,
    guesses: &mut usize,
) {
    let char_set: Vec<char> = char_set.chars().collect();
    let char_set_len = char_set.len();
    let mut current_combination = vec![0; max_length];
    let mut current_string = String::with_capacity(max_length);

    let start = Instant::now();

    loop {
        current_string.clear();
        for &index in &current_combination {
            current_string.push(char_set[index]);
        }

        if current_string == password {
            let elapsed = start.elapsed();
            println!(
                "Found {} with {} guesses in {:.6} seconds",
                password,
                guesses,
                elapsed.as_secs_f64()
            );
            return;
        }

        *guesses += 1;

        // Increment the combination
        let mut pos = max_length as isize - 1;
        while pos >= 0 {
            current_combination[pos as usize] += 1;
            if current_combination[pos as usize] == char_set_len {
                current_combination[pos as usize] = 0;
                pos -= 1;
            } else {
                break;
            }
        }
    }
}

fn main() {
    let char_set = "1234567890";
    let mut password = String::new();
    let mut max_length_str = String::new();

    println!("Password: ");
    std::io::stdin().read_line(&mut password).unwrap();
    let password = password.trim();

    println!("Length: ");
    std::io::stdin().read_line(&mut max_length_str).unwrap();
    let max_length: usize = max_length_str.trim().parse().unwrap();

    let mut guesses = 0;
    generate_combinations(char_set, max_length, password, &mut guesses);
}
